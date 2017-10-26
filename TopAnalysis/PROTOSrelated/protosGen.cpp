#include <iostream>
#include <fstream>
#include <vector>
#include "TLorentzVector.h"
#include <stdio.h>
#include <random>
#include "protosDef.h"
#include "getAngles.cpp"
#include "construction.cpp"

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

    for (int i = 0; i < sT.size(); i++) {
        TLorentzVector top;
        TLorentzVector W;
        TLorentzVector bottom;
        TLorentzVector quarkSpec;
        TLorentzVector lepton;
        TLorentzVector neutrino;
        TLorentzVector buffer;
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
        
        neutrino = getNeutrino(sT[i]);
        lepton = getLepton(sT[i]);
        bottom = getBottom(sT[i]);
        quarkSpec = getQuarkSpec(sT[i]);
        
        buffer = quarkSpec;
        top = neutrino + bottom + lepton;
        
        buffer.Boost(-top.BoostVector());
        quarkSpecT = buffer; //Get Spectator quark in top rest frame.
        
        buffer = neutrino + lepton; // W = N + lepton
        W = buffer;
        buffer.Boost(-top.BoostVector());
        WT = buffer;
        
        q = WT.Vect(); //Z-axis defined.
        q.SetMag(1);
        N = quarkSpecT.Vect().Cross(q);  //-Y-axis defined. N = st X q.
        N.SetMag(1);
        T = q.Cross(N); //X-axis defined. T = q X N.
        T.SetMag(1);

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
    //rp3->GetUpperRefYaxis()->SetTitle("Entry Ratio");
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
    //rp5->GetUpperRefYaxis()->SetTitle("Entry Ratio");
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
    //rp6->GetUpperRefYaxis()->SetTitle("Entry Ratio");
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


