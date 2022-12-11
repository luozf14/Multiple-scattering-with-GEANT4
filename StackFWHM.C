#include <iostream>
#include <vector>
#include <memory>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>

void StackFWHM()
{
    Double_t gr1_fx1001[10] = {
        0.5,
        1,
        1.5,
        2,
        2.5,
        3,
        3.5,
        4,
        4.5,
        5};
    Double_t gr1_fy1001[10] = {
        4.58643,
        2.98088,
        2.16561,
        1.70893,
        1.40455,
        1.2244,
        1.05885,
        0.938896,
        0.842555,
        0.795329};
    Double_t gr1_fex1001[10] = {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0};
    Double_t gr1_fey1001[10] = {
        0.0926163,
        0.0383418,
        0.0237018,
        0.0176422,
        0.0138804,
        0.0114082,
        0.0102536,
        0.00854168,
        0.00752072,
        0.00726581};
    TGraphErrors *gr1 = new TGraphErrors(10, gr1_fx1001, gr1_fy1001, gr1_fex1001, gr1_fey1001);
    gr1->SetName("gr1");
    gr1->SetTitle("FWHM vs Beam energy (304SS)");
    gr1->SetFillStyle(1000);

    Double_t gr1_fx1002[10] = {
        0.5,
        1,
        1.5,
        2,
        2.5,
        3,
        3.5,
        4,
        4.5,
        5};
    Double_t gr1_fy1002[10] = {
        2.3454,
        1.33726,
        0.949719,
        0.765742,
        0.626873,
        0.546254,
        0.466081,
        0.420364,
        0.386679,
        0.342894};
    Double_t gr1_fex1002[10] = {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0};
    Double_t gr1_fey1002[10] = {
        0.026064,
        0.0130399,
        0.00862947,
        0.0069848,
        0.00546045,
        0.00472162,
        0.00396753,
        0.00353016,
        0.00334612,
        0.00293755};
    TGraphErrors *gr2 = new TGraphErrors(10, gr1_fx1002, gr1_fy1002, gr1_fex1002, gr1_fey1002);
    gr2->SetName("gr2");
    gr2->SetTitle("FWHM vs Beam energy (Beryllium)");
    gr2->SetFillStyle(1000);

    gr1->SetMarkerStyle(21);
    gr1->SetMarkerSize(0.5);
    gr1->SetLineColor(kBlue);
    gr2->SetMarkerStyle(21);
    gr2->SetMarkerSize(0.5);
    gr2->SetLineColor(8);

    TCanvas *c2 = new TCanvas("c2", "c2", 1200, 2000);
    c2->cd(1);
    gr1->Draw("acp");
    gr2->Draw("same");

}
