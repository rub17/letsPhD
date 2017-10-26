#include <iostream>
#include <fstream>
#include <vector>
#include "TLorentzVector.h"
#include <stdio.h>
#include <random>
#include "protosDef.h"
#include "construction.cpp"
#include "getAngles.cpp"

void polarizedDecay()
{
    gROOT->Reset();
    std::ifstream inStream("../Events/tbj.unw");
    std::vector<singleTop> sT;
    
    while (!inStream.eof()) {
        singleTop a;
        inStream >> a;
        sT.push_back(a);
    }
    
    for (int i = 0; i < sT.size(); i++) {
        TLorentzVector top;
        TLorentzVector W;
        TLorentzVector bottom;
        TLorentzVector quarkSpec;
        TLorentzVector lepton;
        TLorentzVector neutrino;
        TLorentzVector WT;
        TLorentzVector quarkSpecT;
        double cosTheta;
        double cosThetaStar;
        double phi;
        double phiStar;
        double cosThetaX;
        TVector3 T; //X-axis
        TVector3 N; //-Y-axis
        TVector3 q; //Z-axis

    }
    
}
        
