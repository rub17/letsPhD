#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include "TLorentzVector.h"
#include <stdio.h>

struct substrate{
    int particleId;
    int color;
    int flow;
    double pZ;
};
struct product{
    int particleId;
    int color;
    int flow;
    double pX;
    double pY;
    double pZ;
};
struct singleTop{
    int eventNumber;
    double weight;
    double unknown;
    substrate qg[2];
    product bblvq[5];
};

std::istream& operator>> (std::istream& is, substrate &p)
{
    is >> p.particleId >> p.color >> p.flow >> p.pZ;
    return is;
};

std::istream& operator>> (std::istream& is, product &p)
{
    is >> p.particleId >> p.color >> p.flow >> p.pX >> p.pY >> p.pZ;
    return is;
};
std::istream& operator>> (std::istream& is, singleTop &st)
{
    is >> st.eventNumber >> st.weight >> st.unknown;
    is >> st.qg[0];
    is >> st.qg[1];
    for (int i = 0; i < 5; i++) {
        is >> st.bblvq[i];
    }
    return is;
};

//double particleMass(int id)
//{
//
//    switch (id) {
//        case <#constant#>:
//            <#statements#>
//            break;
//
//        default:
//            break;
//    }
//}
void regen()
{
    gROOT->Reset();
    std::ifstream inStream("tbj.unw");
    std::vector<singleTop> sT;
    
    while (!inStream.eof()) {
        singleTop a;
        inStream >> a;
        sT.push_back(a);
    }

    std::vector<TLorentzVector> neutrinos;
    std::vector<TLorentzVector> leptons;
    std::vector<TLorentzVector> bottoms;
    TLorentzVector currentParticle;
    
    for (int i = 0; i < sT.size(); i++) {
        TLorentzVector currentParticle;
        currentParticle.SetPxPyPzE(sT[i].bblvq[0].pX,
                                   sT[i].bblvq[0].pY,
                                   sT[i].bblvq[0].pZ,
                                   sqrt(pow(sT[i].bblvq[0].pX,2)
                                   +pow(sT[i].bblvq[0].pY,2)
                                   +pow(sT[i].bblvq[0].pZ,2)));
        neutrinos.push_back(currentParticle);
        
        currentParticle.SetPxPyPzE(sT[i].bblvq[1].pX,
                                   sT[i].bblvq[1].pY,
                                   sT[i].bblvq[1].pZ,
                                   sqrt(pow(sT[i].bblvq[1].pX,2)
                                        +pow(sT[i].bblvq[1].pY,2)
                                        +pow(sT[i].bblvq[1].pZ,2)));
        leptons.push_back(currentParticle);
        
        currentParticle.SetPxPyPzE(sT[i].bblvq[2].pX,
                                   sT[i].bblvq[2].pY,
                                   sT[i].bblvq[2].pZ,
                                   sqrt(pow(sT[i].bblvq[2].pX,2)
                                        +pow(sT[i].bblvq[2].pY,2)
                                        +pow(sT[i].bblvq[2].pZ,2)));
        bottoms.push_back(currentParticle);
    }
    
    
    std::cout << sT[150].eventNumber << std::endl;
    std::cout << neutrinos[0].Pt() << std::endl;
    std::cout << leptons[111].Px() << std::endl;
    std::cout << bottoms[222].Px() << std::endl;

}
