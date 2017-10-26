#include "protosDef.h"

TLorentzVector getNeutrino(singleTop sT){
    TLorentzVector neutrino;
    neutrino.SetPxPyPzE(sT.bblvq[0].pX,
                        sT.bblvq[0].pY,
                        sT.bblvq[0].pZ,
                        sqrt(pow(sT.bblvq[0].pX,2)
                             + pow(sT.bblvq[0].pY,2)
                             + pow(sT.bblvq[0].pZ,2)));
    return neutrino;
}

TLorentzVector getLepton(singleTop sT){
    TLorentzVector lepton;
    lepton.SetPxPyPzE(sT.bblvq[1].pX,
                      sT.bblvq[1].pY,
                      sT.bblvq[1].pZ,
                      sqrt(pow(sT.bblvq[1].pX,2)
                           +pow(sT.bblvq[1].pY,2)
                           +pow(sT.bblvq[1].pZ,2)
                           +pow(particleMass(sT.bblvq[1].particleId),2)));
    return lepton;
}

TLorentzVector getBottom(singleTop sT){
    TLorentzVector bottom;
    bottom.SetPxPyPzE(sT.bblvq[2].pX,
                      sT.bblvq[2].pY,
                      sT.bblvq[2].pZ,
                      sqrt(pow(sT.bblvq[2].pX,2)
                           +pow(sT.bblvq[2].pY,2)
                           +pow(sT.bblvq[2].pZ,2)
                           +pow(particleMass(sT.bblvq[2].particleId),2)));
    return bottom;
}

TLorentzVector getQuarkSpec(singleTop sT){
    TLorentzVector quarkSpec;
    quarkSpec.SetPxPyPzE(sT.bblvq[4].pX,
                         sT.bblvq[4].pY,
                         sT.bblvq[4].pZ,
                         sqrt(pow(sT.bblvq[4].pX,2)
                              +pow(sT.bblvq[4].pY,2)
                              +pow(sT.bblvq[4].pZ,2)));
    return quarkSpec;
}

