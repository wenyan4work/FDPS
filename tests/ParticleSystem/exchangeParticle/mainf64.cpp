#include <iostream>
#include <vector>
#include <cassert>

#include "FDPS_particle_simulator.hpp"
#include "../../basic_particle.hpp"
#include "../check_particle_system.hpp"

template <class Tptcl>
void generateSphere(PS::U32 seed,
                    PS::S32 ntot,
                    Tptcl & system,
                    PS::F64 radius,
                    PS::F64vec center)
{
    PS::S32 rank = PS::Comm::getRank();
    PS::S32 size = PS::Comm::getNumberOfProc();

    PS::S32 ibgn = (ntot * rank) / size;
    PS::S32 iend = (ntot * (rank + 1)) / size;
    PS::S32 nloc = iend - ibgn;    
    PS::MT::init_genrand(seed);
    for(PS::S32 i = 0; i < nloc; i++)
        system[i].generateSphere(i+ibgn, radius, center);
    system.setNumberOfParticleLocal(nloc);
    
    return;
}                    

template <class Tptcl>
void writeAscii(Tptcl & system)
{
    PS::S32 rank = PS::Comm::getRank();
    char filename[1024];
    sprintf(filename, "snap.init.%04d", rank);

    PS::S32 nloc = system.getNumberOfParticleLocal();
    FILE *fp = fopen(filename, "w");
    for(PS::S32 i = 0; i < nloc; i++)
        system[i].writeAscii(fp);
    fclose(fp);

    return;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc,&argv); 
    PS::Initialize(argc, argv);

    PS::S32    nmem = 131072;
    PS::S32    ntot = 83927;
    PS::F64    prad = 3.1d;
    PS::F64vec pcen(1.d, -10.d, 199.8d);
    PS::U32    seed = PS::Comm::getRank();    
    PS::S32    code = 0;
    
    PS::DomainInfo dinfo;
    PS::ParticleSystem<BasicParticle64> bp;

    dinfo.initialize();
    bp.initialize();
    bp.createParticle(nmem);    
    generateSphere(seed, ntot, bp, prad, pcen);

    dinfo.decomposeDomainAll(bp);
    bp.exchangeParticle(dinfo);

    code = (bp.checkExchangeParticleAllParticleInside(dinfo)) ? code : (code | 1);    
    code = (bp.checkExchangeParticleSumOfNumberOfParticle(dinfo, ntot)) ? code : (code | (1 << 1));

    PS::Finalize();       
    MPI_Finalize();

    return code;
}
