#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include "TLorentzVector.h"
#include <stdio.h>
#include <random>

double getCosTheta (TLorentzVector restParticle, TLorentzVector particleOne,TLorentzVector particleTwo) {//Due to simpler definitions, with theta angle we can simply give the two particles and boost them into the wanted frame. That part is all done by the routine. The resulting angle between them will be the theta angle (z-axis is the W boson).

    TLorentzVector restFrame = restParticle;
    TLorentzVector particle1 = particleOne;
    TLorentzVector particle2 = particleTwo;

    particle1.Boost(-restFrame.BoostVector());
    particle2.Boost(-restFrame.BoostVector());

    double Angle = cos(particle1.Angle(particle2.Vect()));

    return Angle;
};

double getPhi (TLorentzVector restParticle, TLorentzVector particle,TVector3 x_axis, TVector3 y_axis,TVector3 z_axis) { //Due to the relatively complex setup, we will need x-axis in the right frame to be provided for the routine; in other words, it does not boost the last argument input. Double check if the x-axis is boosted in the frame that's described in the first input.
    
    TLorentzVector restFrame = restParticle;
    TLorentzVector p = particle;
    TVector3 x = x_axis;
    TVector3 y = y_axis;
    TVector3 z = z_axis;
    TVector3 pVect;
    
    p.Boost(-restFrame.BoostVector());
    pVect = p.Vect();
    pVect = pVect - pVect.Dot(z)*z;
    double pVect_X = pVect.Dot(x);
    double pVect_Y = pVect.Dot(y);
    double Angle = atan2(pVect_Y,pVect_X);
    
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

void protosGen()
{
    gROOT->Reset();
    std::ifstream inStream("../Events/tbj.wgt");
    std::vector<singleTop> sT;
    
    while (!inStream.eof()) {
        singleTop a;
        inStream >> a;
        sT.push_back(a);
    }
    
    auto *h3 = new TH1F("CosTheta", "CosTheta", 30, -1, 1);
    h3 -> SetMarkerStyle(2);
    h3 -> GetXaxis()->SetTitle("cos(#theta)");
    
    auto *h4 = new TH1F("Phi", "Phi", 30, -3.5, 3.5);
    h4 -> GetXaxis()->SetTitle("#phi");
    
    auto *h5 = new TH1F("CosThetaStar", "CosThetaStar", 30, -1, 1);
    h5 -> SetMarkerStyle(2);
    h5 -> GetXaxis()->SetTitle("cos(#theta *)");
    
    auto *h6 = new TH1F("PhiStar", "PhiStar", 30, -3.5, 3.5);
    h6 -> SetMarkerStyle(2);
    h6 -> GetXaxis()->SetTitle("#phi *");
    
    auto *h7 = new TH1F("CosThetaX", "CosThetaX", 30, -1, 1);
    h7 -> SetMarkerStyle(2);
    h7 -> GetXaxis()->SetTitle("cos(#theta x)");


//    std::vector<TLorentzVector> neutrinos;
//    std::vector<TLorentzVector> leptons;
//    std::vector<TLorentzVector> bottom;
//    std::vector<TLorentzVector> top;
//    std::vector<TLorentzVector> W;
//    std::vector<TLorentzVector> quarkSpec;
//
//    std::vector<double> cosTheta;
//    std::vector<double> phi;
//    std::vector<double> cosThetaStar;
//    std::vector<double> phiStar;
    
    //TF1 *f1 = new TF1("theta","1 + cos(x)",0,M_PI);
   // TRandom *r1 = new TRandom();
    for (int i = 0; i < sT.size(); i++) {
        TLorentzVector top;
        TLorentzVector W;
        TLorentzVector bottom;
        TLorentzVector quarkSpec;
        TLorentzVector lepton;
        TLorentzVector neutrino;
        TLorentzVector currentParticle;
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
        
        currentParticle.SetPxPyPzE(sT[i].bblvq[0].pX,
                                   sT[i].bblvq[0].pY,
                                   sT[i].bblvq[0].pZ,
                                   sqrt(pow(sT[i].bblvq[0].pX,2)
                                   +pow(sT[i].bblvq[0].pY,2)
                                   +pow(sT[i].bblvq[0].pZ,2)));
        neutrino = currentParticle;
        
        currentParticle.SetPxPyPzE(sT[i].bblvq[1].pX,
                                   sT[i].bblvq[1].pY,
                                   sT[i].bblvq[1].pZ,
                                   sqrt(pow(sT[i].bblvq[1].pX,2)
                                        +pow(sT[i].bblvq[1].pY,2)
                                        +pow(sT[i].bblvq[1].pZ,2)
                                        +pow(particleMass(sT[i].bblvq[1].particleId),2)));
        lepton = currentParticle;
        
        currentParticle.SetPxPyPzE(sT[i].bblvq[2].pX,
                                   sT[i].bblvq[2].pY,
                                   sT[i].bblvq[2].pZ,
                                   sqrt(pow(sT[i].bblvq[2].pX,2)
                                        +pow(sT[i].bblvq[2].pY,2)
                                        +pow(sT[i].bblvq[2].pZ,2)
                                        +pow(particleMass(sT[i].bblvq[2].particleId),2)));
        bottom = currentParticle;
        
        currentParticle = neutrino + bottom + lepton; //Top = Neutrino +Bottom  + Lepton
        top = currentParticle;
        
        currentParticle.SetPxPyPzE(sT[i].bblvq[4].pX,
                                   sT[i].bblvq[4].pY,
                                   sT[i].bblvq[4].pZ,
                                   sqrt(pow(sT[i].bblvq[4].pX,2)
                                        +pow(sT[i].bblvq[4].pY,2)
                                        +pow(sT[i].bblvq[4].pZ,2)));
        quarkSpec = currentParticle;
        currentParticle.Boost(-top.BoostVector());
        quarkSpecT = currentParticle; //Get Spectator quark in top rest frame.
        
        currentParticle = neutrino + lepton; // W = N + lepton
        W = currentParticle;
        currentParticle.Boost(-top.BoostVector());
        WT = currentParticle;
        q = WT.Vect(); //Z-axis defined.
        q.SetMag(1);
        
        N = quarkSpecT.Vect().Cross(q);  //N = st X q; N-->y;
        N.SetMag(1);
        T = q.Cross(N); // T = q X N; T-->x
        T.SetMag(1);
        
//        numberCosTheta,numberCosThetaStar = cos(f1->GetRandom());
//        anglePhi,anglePhiStar = r1->Uniform(0,2*M_PI);

        cosTheta = getCosTheta(top,quarkSpec,W);

        phi = getPhi(top,quarkSpec,T,-N,q);// phi = angle between x axis and W's projection on x-y plane.
        cosThetaStar = getCosTheta(W,lepton,-bottom); //bottom quark points at the opposite direction of W in top.
        phiStar = getPhi(W,lepton,T,-N,q);// phi* = angle between x axis and l's projection on x-y plane (W frame).
        cosThetaX = getCosTheta(top,quarkSpec,lepton);
        
        h3->Fill(cosTheta,sT[i].weight);
        h4->Fill(phi,sT[i].weight);
        h5->Fill(cosThetaStar,sT[i].weight);
        h6->Fill(phiStar,sT[i].weight);
        h7->Fill(cosThetaX,sT[i].weight);
    }
    
    auto *c3 = new TCanvas("c3","Top Rest Frane");
    c3->Divide(2,1);
    c3->cd(1);
    h3->Scale(1/h3->Integral());
    h3->SetMinimum(0);
    h3->Draw("P");
    h3->Fit("pol2");
    c3->cd(1)->Clear();
    auto rp3 = new TRatioPlot(h3);
    rp3->Draw();
    rp3->GetLowerRefYaxis()->SetTitle("ratio");
    rp3->GetUpperRefYaxis()->SetTitle("Entry Ratio");
    c3->cd(1)->Update();
    
    c3->cd(2);
    h4->Scale(1/h4->Integral());
    h4->SetMinimum(0);
    h4->Draw("H");

    auto *c4 = new TCanvas("c4","W Rest Frame");
    c4->Divide(2,1);
    c4->cd(1);
    h5->Scale(1/h5->Integral());
    h5->SetMinimum(0);
    h5 ->Draw("P");
    h5->Fit("pol2");
    c4->cd(1)->Clear();
    auto rp5 = new TRatioPlot(h5);
    rp5->Draw();
    rp5->GetLowerRefYaxis()->SetTitle("ratio");
    rp5->GetUpperRefYaxis()->SetTitle("Entry Ratio");
    c4->cd(1)->Update();
    
    c4->cd(2);
    h6->Scale(1/h6->Integral());
    h6->SetMinimum(0);
    h6->Draw("P");
    h6->Fit("gaus");
    c4->cd(2)->Clear();
    auto rp6 = new TRatioPlot(h6);
    rp6->Draw();
    rp6->GetLowerRefYaxis()->SetTitle("ratio");
    rp6->GetUpperRefYaxis()->SetTitle("Entry Ratio");
    c4->cd(2)->Update();
    
    auto *c5 = new TCanvas("c5","ThetaX");
    h7->Scale(1/h7->Integral());
    h7->SetMinimum(0);
    h7->Draw("P");
    h7->Fit("pol2");
    c5 -> Clear();
    auto rp7 = new TRatioPlot(h7);
    rp7->Draw();
    rp7->GetLowerRefYaxis()->SetTitle("ratio");
    rp7->GetUpperRefYaxis()->SetTitle("Entry Ratio");
    c5->Update();
    
}


