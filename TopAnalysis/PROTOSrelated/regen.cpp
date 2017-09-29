#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include "TLorentzVector.h"
#include <stdio.h>

double getAngleCosTheta (TLorentzVector restParticle, TLorentzVector particleOne,TLorentzVector particleTwo) {

    TLorentzVector restFrame = restParticle;
    TLorentzVector particle1 = particleOne;
    TLorentzVector particle2 = particleTwo;

    particle1.Boost(-restFrame.BoostVector());
    particle2.Boost(-restFrame.BoostVector());

    double Angle = cos(particle1.Angle(particle2.Vect()));

    return Angle;
};

double getAnglePhi (TLorentzVector restParticle, TLorentzVector particleOne,TVector3 x_axis) {

    TLorentzVector restFrame = restParticle;
    TLorentzVector particle1 = particleOne;
    TVector3 x = x_axis;
    TVector3 part;
    
    particle1.Boost(-restFrame.BoostVector());
    part = particle1.Vect();
    part.SetZ(0);
    
    double Angle = part.Angle(x_axis);

    return Angle;
};

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
    double eventQ;
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
    is >> st.eventNumber >> st.weight >> st.eventQ;
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
    std::vector<TLorentzVector> W;
    std::vector<TLorentzVector> quarkSpec;

    std::vector<double> cosTheta;
    std::vector<double> phi;
    std::vector<double> cosThetaStar;
    std::vector<double> phiStar;
    

    for (int i = 0; i < sT.size(); i++) {
        TLorentzVector currentParticle;
        double numberCosTheta;
        double numberCosThetaStar;
        double anglePhi;
        double anglePhiStar;

        
        TVector3 T;
        TVector3 N;
        TVector3 Wproj;
        
        currentParticle.SetPxPyPzE(sT[i].bblvq[4].pX,
                                   sT[i].bblvq[4].pY,
                                   sT[i].bblvq[4].pZ,
                                   sqrt(pow(sT[i].bblvq[4].pX,2)
                                        +pow(sT[i].bblvq[4].pY,2)
                                        +pow(sT[i].bblvq[4].pZ,2)));
        quarkSpec.push_back(currentParticle);
        
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
        
        currentParticle = neutrinos.back() + bottom.back() + leptons.back(); //Top = Neutrino +
                                                                             //Bottom  + Lepton
        top.push_back(currentParticle);
        
        currentParticle = neutrinos.back() + leptons.back(); // W = N + lepton
        W.push_back(currentParticle);
        
        numberCosTheta = getAngleCosTheta(top.back(),quarkSpec.back(),W.back());
        cosTheta.push_back(numberCosTheta);
        
        N = quarkSpec.back().Vect().Cross(W.back().Vect());  //N = st X q; N-->y;
        T = W.back().Vect().Cross(N); // T = q X N; T-->x
        anglePhi = getAnglePhi(top.back(),W.back(),T);// phi = angle between x axis and W's projection on x-y plane.
        phi.push_back(anglePhi);

        numberCosThetaStar = getAngleCosTheta(W.back(),quarkSpec.back(),leptons.back());
        cosThetaStar.push_back(numberCosThetaStar);
        
        anglePhiStar = getAnglePhi(W.back(),leptons.back(),T);// phi* = angle between x axis and l's projection on x-y plane (W                                         frame).
        phiStar.push_back(anglePhiStar);
    }
    

//    TH1F *h1 = new TH1F("Top", "Top Events", 1000, 100, 250);
//    TH1F *h2 = new TH1F("QValue", "Event Q Value", 100, 100, 1000);
//
//    for (int i=0; i < top.size(); i++) {
//        h1->Fill(top[i].M());
//        h2->Fill(sT[i].eventQ);
//    }

    
    TH1F *h3 = new TH1F("CosTheta", "CosTheta", 50, -1.5, 1.5);
    TH1F *h4 = new TH1F("Phi", "Phi", 50, -3.5, 3.5);
    TH1F *h5 = new TH1F("CosThetaStar", "CosThetaStar", 50, -1.5, 1.5);
    TH1F *h6 = new TH1F("PhiStar", "PhiStar", 50, -3.5, 3.5);
    
    for (int i=0; i < top.size(); i++) {
        h3->Fill(cosTheta[i]);
        h4->Fill(phi[i]);
        h5->Fill(cosThetaStar[i]);
        h6->Fill(phiStar[i]);
    }

//    TCanvas *c1 = new TCanvas("c1","c1");
//    h1->Draw();
//    TCanvas *c2 = new TCanvas("c2","c2");
//    h2->Draw();
    
    TCanvas *c3 = new TCanvas("c3","Top Rest Frane");
    c3->Divide(2,1);
    c3->cd(1);
    h3 ->Draw();
    c3->cd(2);
    h4->Draw();


    TCanvas *c4 = new TCanvas("c4","W Rest Frame");
    c4->Divide(2,1);
    c4->cd(1);
    h5 ->Draw();
    c4->cd(2);
    h6->Draw();
}


