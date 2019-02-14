#include <cassert>
#include <iostream>
#include <random>
#include <type_traits>
#include <vector>

#include "particle_simulator.hpp"

class forceNearEP {
  public:
    PS::S32 nb = 0;
    void clear() {}
};

/*FullParticle class for FDPS*/
class SphereFP {
  public:
    PS::S32 gid;
    PS::F64vec3 pos;
    PS::F64 RSearch;
    PS::S32 nb = 0;

    PS::F64vec getPos() const { return this->pos; }

    void copyFromForce(const forceNearEP &force) { nb = force.nb; }

    void setPos(const PS::F64vec3 &pos_) { this->pos = pos_; }

    void writeAscii(FILE *fp) const {
        const double radius = 1;
        fprintf(fp, "S\t%8d\t%.17g\t%.17g\t%.17g\t%.17g\n", this->gid, radius, this->pos[0], this->pos[1],
                this->pos[2]);
    }
};

class SphereNearForceEP {
  public:
    PS::S32 gid;
    PS::F64vec3 pos;
    PS::F64 RSearch;

    PS::F64vec getPos() const { return this->pos; }

    void setPos(const PS::F64vec3 &pos) { this->pos = pos; }

    PS::F64 getRSearch() const { return this->RSearch; }

    void copyFromFP(const SphereFP &fp) {
        gid = fp.gid;
        pos = fp.pos;
        RSearch = fp.RSearch;
    }
};

class SphereNearForceEPJ {
  public:
    PS::S32 gid;
    PS::F64vec3 pos;
    PS::F64 RSearch;

    PS::F64vec getPos() const { return this->pos; }

    void setPos(const PS::F64vec3 &pos) { this->pos = pos; }

    PS::F64 getRSearch() const { return this->RSearch; }

    void copyFromFP(const SphereFP &fp) {
        gid = fp.gid;
        pos = fp.pos;
        RSearch = fp.RSearch;
    }
};

class CalcNearForceEPIJ {
  public:
    // constructor
    CalcNearForceEPIJ() {}

    // copy constructor
    CalcNearForceEPIJ(const CalcNearForceEPIJ &obj) {}

    void operator()(const SphereNearForceEP *const ep_i, const PS::S32 Nip, const SphereNearForceEPJ *const ep_j,
                    const PS::S32 Njp, forceNearEP *const force) {

        for (PS::S32 i = 0; i < Nip; ++i) {
            force[i].clear();
            auto &sphereI = ep_i[i];
            for (PS::S32 j = 0; j < Njp; ++j) {
                auto &sphereJ = ep_j[j];
                force[i].nb++;
            }
        }
    }
};

int main(int argc, char **argv) {

    static_assert(std::is_trivially_destructible<PS::F64vec3>::value);
    static_assert(std::is_trivially_copyable<PS::F64vec3>::value);
    static_assert(std::is_trivially_copyable<PS::F64vec2>::value);

    PS::Initialize(argc, argv);

    PS::ParticleSystem<SphereFP> systemSP;
    systemSP.initialize();
    PS::DomainInfo dinfo;

    PS::TreeForForceShort<forceNearEP, SphereNearForceEP, SphereNearForceEPJ>::Symmetry treeSymmetry;
    PS::TreeForForceShort<forceNearEP, SphereNearForceEP, SphereNearForceEPJ>::Scatter treeScatter;
    CalcNearForceEPIJ calcNearForceFtr;

    const int nLocal = 2000000;
    systemSP.setNumberOfParticleLocal(nLocal);
#pragma omp parallel for
    for (int i = 0; i < nLocal; i++) {
        systemSP[i].pos[0] = sin(i) * 100;
        systemSP[i].pos[1] = cos(i) * 100;
        systemSP[i].pos[2] = sin(cos(i)) * 100;
        systemSP[i].RSearch = 10.0;
    }

    PS::Comm::barrier();

    dinfo.initialize();
    dinfo.setBoundaryCondition(PS::BOUNDARY_CONDITION_PERIODIC_XYZ);
    dinfo.setPosRootDomain(PS::F64vec3(0, 0, 0), PS::F64vec3(100, 100, 100));
    systemSP.adjustPositionIntoRootDomain(dinfo);

    PS::Comm::barrier();
    dinfo.decomposeDomainAll(systemSP);
    systemSP.exchangeParticle(dinfo);
    assert(systemSP.getNumberOfParticleLocal() > 0);

    PS::Comm::barrier();
    treeSymmetry.initialize(nLocal * 4);
    treeSymmetry.calcForceAllAndWriteBack(calcNearForceFtr, systemSP, dinfo);

    PS::Comm::barrier();
    treeScatter.initialize(nLocal * 4);
    treeScatter.calcForceAllAndWriteBack(calcNearForceFtr, systemSP, dinfo);

    PS::Comm::barrier();
    PS::Finalize();
    return 0;
}
