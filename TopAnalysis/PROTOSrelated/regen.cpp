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

double particleMass(int id)
{
    switch (id) {
        case 1:
        case -1:
            return 0.0048;
        case 2:
        case -2:
            return 0.0023;
        case 3:
        case -3:
            return 0.095;
        case 4:
        case -4:
            return 1.29;
        case 5:
        case -5:
            return 4.18;
        case 6:
        case -6:
            return 172.44;
        case 11:
        case -11:
            return 0.000511;
        case 13:
        case -13:
            return 0.10566;
        case 15:
        case -15:
            return 1.7768;
    }
    return 0;
};

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
    std::vector<TLorentzVector> bottom;
    std::vector<TLorentzVector> top;
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
                                        +pow(sT[i].bblvq[1].pZ,2)
                                        +pow(particleMass(sT[i].bblvq[1].particleId),2)));
        leptons.push_back(currentParticle);
        
        currentParticle.SetPxPyPzE(sT[i].bblvq[2].pX,
                                   sT[i].bblvq[2].pY,
                                   sT[i].bblvq[2].pZ,
                                   sqrt(pow(sT[i].bblvq[2].pX,2)
                                        +pow(sT[i].bblvq[2].pY,2)
                                        +pow(sT[i].bblvq[2].pZ,2)
                                        +pow(particleMass(sT[i].bblvq[2].particleId),2)));
        bottom.push_back(currentParticle);
        
        currentParticle = neutrinos.back() + bottom.back() + leptons.back();
        top.push_back(currentParticle);
    }
    
    TH1F *h1 = new TH1F("Top", "Top Events", 1000, 100, 250);
    for (int i=0; i < top.size(); i++) {
        h1->Fill(top[i].M());
    }
    h1->Draw();
    


}
