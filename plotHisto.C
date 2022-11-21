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
    TFile* file = new TFile("Data.root", "READ");

    auto tree = (TTree *)file->GetDirectory("ntuple")->Get("Ntuple1");

    Int_t trackID;
    Char_t *particleName;
    Double_t energyDeposit, positionX, positionY;

    tree->SetBranchAddress("TrackID", &trackID);
    tree->SetBranchAddress("ParticleName", &particleName);
    tree->SetBranchAddress("EnergyDeposit", &energyDeposit);
    tree->SetBranchAddress("Position.x", &positionX);
    tree->SetBranchAddress("Position.y", &positionY);

    TH1D *h1PositionX = new TH1D("h1PositionX", "Position on X-axis", 1000, -20, 20);
    h1PositionX->SetXTitle("PositionX [mm]");

    TH1D *h1PositionY = new TH1D("h1PositionY", "Position on Y-axis", 1000, -20, 20);
    h1PositionY->SetXTitle("PositionY [mm]");

    Int_t nEntries = tree->GetEntries();
    for (int i = 0; i < nEntries; i++)
    {
        tree->GetEntry(i);
        h1PositionX->Fill(positionX);
        h1PositionY->Fill(positionY);
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 2048, 768);
    c1->Divide(2,1);
    c1->cd(1);
    h1PositionX->Draw();
    c1->cd(2);
    h1PositionY->Draw();
    c1->Update();
}