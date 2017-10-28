//
//  polarizedDecay.cpp
//
//  For takign the top generations from PROTOS and decay it with the 4 angle formula.
//
//  Created by Runyu on 10/26/17.
//


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
    
    TF1 *fTheta = new TF1("fTheta","1+cos(x)",0,2);
    TRandom *r = new TRandom();
    
    for (int i = 0; i < sT.size(); i++) {
        TLorentzVector top;
        TLorentzVector W;
        TLorentzVector bottom;
        TLorentzVector quarkSpec;
        TLorentzVector lepton;
        TLorentzVector neutrino;
        TLorentzVector WT;
        TLorentzVector quarkSpecT;
        double cosTheta = cos(fTheta->GetRandom());
        double cosThetaStar = cos(fTheta->GetRandom());
        double phi = r->Uniform(-M_PI,M_PI);
        double phiStar = r->Uniform(-M_PI,M_PI);
        TVector3 T; //X-axis
        TVector3 N; //-Y-axis
        TVector3 Z_axis; //Z-axis

        neutrino = getNeutrino(sT[i]);
        lepton = getLepton(sT[i]);
        bottom = getBottom(sT[i]);
        quarkSpec = getQuarkSpec(sT[i]);
        top = bottom + lepton + neutrino;
        
        quarkSpec.Boost(-top);
        Z_axis = -1*quarkSpec.Vect(); //Say we define top as
    }
    
}
        
