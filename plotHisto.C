#include <iostream>
#include <vector>
#include <memory>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>

void plotHisto()
{
    Double_t sigma[10], sigmaError[10];
    TGraphErrors *gr1 = new TGraphErrors(0);
    gr1->SetNameTitle("gr1", "FWHM vs Beam energy (Dry air at 1 atm)");
    gr1->GetXaxis()->SetTitle("Beam energy [MeV]");
    gr1->GetXaxis()->CenterTitle();
    gr1->GetYaxis()->SetTitle("FWHM [mm]");
    gr1->GetYaxis()->CenterTitle();
    for (int i = 0; i < 10; i++)
    {
        Double_t energy = 0.5 + 0.5 * (double)i;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << energy;
        std::string energyStr = stream.str();
        replace(energyStr.begin(), energyStr.end(), '.', '_');
        std::string fileName = "Data~" + energyStr + ".root";
        TFile *file = new TFile(fileName.c_str(), "READ");

        auto tree = (TTree *)file->GetDirectory("ntuple")->Get("Ntuple1");

        Int_t trackID;
        Char_t *particleName;
        Double_t energyDeposit, positionX, positionY;

        tree->SetBranchAddress("TrackID", &trackID);
        tree->SetBranchAddress("ParticleName", &particleName);
        tree->SetBranchAddress("EnergyDeposit", &energyDeposit);
        tree->SetBranchAddress("Position.x", &positionX);
        tree->SetBranchAddress("Position.y", &positionY);

        TH1D *h1PositionX = new TH1D("h1PositionX", "Position on X-axis", 5000, -20, 20);
        h1PositionX->SetXTitle("PositionX [mm]");

        TH1D *h1PositionY = new TH1D("h1PositionY", "Position on Y-axis", 5000, -20, 20);
        h1PositionY->SetXTitle("PositionY [mm]");

        TH1D *h1PositionXStrip = new TH1D("h1PositionXStrip", "Position on X-axis with 1 mm strips", 40, -20, 20);
        h1PositionXStrip->SetXTitle("PositionX [mm]");

        Int_t frontStripNo;
        Int_t backStripNo;

        Int_t nEntries = tree->GetEntries();
        for (int i = 0; i < nEntries; i++)
        {
            tree->GetEntry(i);
            h1PositionX->Fill(positionX);
            h1PositionY->Fill(positionY);
            frontStripNo = -std::floor((positionX - 20.) / 1.) - 1;
            backStripNo = -std::floor((positionY - 20.) / 1.) - 1;
            h1PositionXStrip->Fill(positionX);
        }


        TF1 *f1 = new TF1("f1", "gaus(0)+pol0(3)", -10,10);
        f1->SetParameters(1500, 0, 1, 1);
        f1->SetParLimits(1, 0, 10);
        f1->SetParLimits(2, 0, 10);
        f1->SetNpx(1000);
        h1PositionXStrip->Fit("f1", "R S L I");
        h1PositionXStrip->GetXaxis()->SetRangeUser(-10,10);
        sigma[i] = f1->GetParameter(2);
        sigmaError[i] = f1->GetParError(2);
        gr1->AddPoint(energy, sigma[i] * 2. * std::sqrt(2. * std::log(2.)));
        gr1->SetPointError(i, 0., sigmaError[i] * 2. * std::sqrt(2. * std::log(2.)));

        TCanvas *c1 = new TCanvas("c1", "c1", 1024, 1024);
        c1->Divide(2,1);
        c1->cd(1);
        h1PositionX->Draw();
        c1->cd(2);
        h1PositionXStrip->Draw();
        c1->Update();
        // c1->SaveAs("c1.png");
        // c1->SaveAs("c1.root");
    }
    TCanvas *c2 = new TCanvas("c2", "c2", 1200, 2000);
    c2->cd(1);
    gr1->SetMarkerStyle(21);
    gr1->SetMarkerSize(0.5);
    gr1->SetLineColor(kRed);
    gr1->Draw("alp");
    c2->SaveAs("c2.png");
}