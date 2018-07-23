#include <TAxis.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TROOT.h>
#include <TTree.h>
#include <TVectorD.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include "ptcut.cc"

// load ptcut.cc macro to do analysis as mCP_anal struct defined there

// calculate pT cut from charge based on approximate angle coming out from
// detector
Double_t calc_pTcut(Double_t q) {
  Double_t c = 299792458.0;
  Double_t det_length = 3.0;
  Double_t width_tolerance = 0.025;
  Double_t Bfield = 3.8;
  Double_t Bfield_R = 3.0;
  Double_t pTcut =
      q * c * 1e-9 * Bfield_R * Bfield / 2.0 *
      TMath::Sqrt(TMath::Power(det_length / width_tolerance, 2) + 1);
  return pTcut;
}

// calculate charge from pT cut based on approximate angle coming out from
// detector
Double_t calc_q(Double_t pT) {
  Double_t c = 299792458.0;
  Double_t det_length = 3.0;
  Double_t width_tolerance = 0.025;
  Double_t Bfield = 3.8;
  Double_t Bfield_R = 3.0;
  Double_t q =
      pT / (c * 1e-9 * Bfield_R * Bfield / 2.0 *
            TMath::Sqrt(TMath::Power(det_length / width_tolerance, 2) + 1));
  return q;
}

void plotgraph() {
  // load up vectors of the masses we have files for and the number of files of
  // each mass
  std::vector<TString> masses;
  std::vector<int> num_files;

  masses.push_back("0.01");
  num_files.push_back(2);

  masses.push_back("0.0540");
  num_files.push_back(2);

  masses.push_back("0.0810");
  num_files.push_back(2);

  masses.push_back("0.219");
  num_files.push_back(4);

  masses.push_back("0.329");
  num_files.push_back(4);

  masses.push_back("0.5");
  num_files.push_back(4);

  masses.push_back("1");
  num_files.push_back(8);

  masses.push_back("2");
  num_files.push_back(8);

  masses.push_back("10");
  num_files.push_back(2);

  std::vector<vector<TString>> files;
  // create lists of file names
  for (std::size_t i = 0; i < masses.size(); i++) {
    std::vector<TString> mass_files;
    for (int j = 1; j <= num_files[i]; j++)
      mass_files.push_back("data/" + masses[i] + "_" + j + ".root");
    if (mass_files.size() > 0) files.push_back(mass_files);
  }

  // vector of pT cuts we will plot
  std::vector<Double_t> pTcuts = {0.1, 0.2, 0.5, 1.0, 2.0};
  // vector of analyses of those pT cuts
  std::vector<std::vector<mCP_anal>> pT_analyses;

  // do analysis with each pT cut and add to master list
  for (std::size_t pT_i = 0; pT_i < pTcuts.size(); pT_i++) {
    // run analysis on each mass size with given pT cut
    Double_t pTcut = pTcuts[pT_i];
    std::vector<mCP_anal> analyses;
    for (std::size_t i = 0; i < files.size(); i++) {
      mCP_anal analysis = analyze_pythia_sim(pTcut, files[i]);
      analyses.push_back(analysis);
    }
    pT_analyses.push_back(analyses);
  }

  // output amount of fb^-1 calculation is based on
  cout << "data: " << data << " fb^-1" << endl;
  cout << endl;

  // output table of analysis result points and acceptance for each pT cut and
  // mass
  for (std::size_t pT_i = 0; pT_i < pTcuts.size(); pT_i++) {
    std::vector<mCP_anal> analyses = pT_analyses[pT_i];
    Double_t pTcut = pTcuts[pT_i];
    // output mCP pT cut used
    cout << "Next table mCP pT cut: " << pTcut << " GeV" << endl;
    cout << std::setfill('=') << std::setw(33) << "" << std::setfill(' ')
         << endl;
    cout << std::setw(7) << "m (GeV)"
         << " " << std::setw(11) << "mCP seen"
         << "   " << std::setw(11) << "acceptance" << endl;
    cout << std::setfill('-') << std::setw(33) << "" << std::setfill(' ')
         << endl;
    for (std::size_t i = 0; i < analyses.size(); i++) {
      mCP_anal anal = analyses[i];
      std::ostringstream mCP_seen_err;
      std::ostringstream acceptance_err;
      mCP_seen_err << "+-" << anal.mCP_seen_err;
      acceptance_err << "+-" << anal.acceptance_err;
      cout << std::setw(5) << anal.mass << "   " << std::setw(11)
           << anal.mCP_seen << "   " << std::setw(11) << anal.acceptance
           << endl;
      cout << std::setw(5) << " "
           << " " << std::setw(13) << mCP_seen_err.str() << " " << std::setw(13)
           << acceptance_err.str() << endl;
    }
    cout << std::setfill('=') << std::setw(33) << "" << std::setfill(' ')
         << endl;
    cout << endl;
  }

  // plot graph of mCP incident on milliQan for each pT cut
  TCanvas *c1 = new TCanvas(TString("mCP_canvas"));
  TMultiGraph *mg = new TMultiGraph();

  // add each graph to a vector for use with legend later
  std::vector<TGraph *> graphs;
  for (std::size_t pT_i = 0; pT_i < pTcuts.size(); pT_i++) {
    std::vector<mCP_anal> analyses = pT_analyses[pT_i];
    Double_t pTcut = pTcuts[pT_i];
    std::vector<Double_t> x;
    std::vector<Double_t> y;
    std::vector<Double_t> ex;
    std::vector<Double_t> ey;
    for (std::size_t i = 0; i < analyses.size(); i++) {
      mCP_anal anal = analyses[i];
      // only add if more than 0 seen since log plot
      if (anal.mCP_seen > 0.0) {
        x.push_back(anal.mass);
        ex.push_back(0.0);
        y.push_back(anal.mCP_seen);
        ey.push_back(anal.mCP_seen_err);
      }
    }
    Int_t n = x.size();
    TGraphErrors *gr =
        new TGraphErrors(n, x.data(), y.data(), ex.data(), ey.data());
    // set colors so they aren't blinding
    gr->SetLineColor(pT_i + 2);
    if (gr->GetLineColor() == 3) gr->SetLineColor(kGreen + 1);
    if (gr->GetLineColor() == 5) gr->SetLineColor(kOrange + 7);
    // set variant line styles
    gr->SetLineStyle(pT_i + 1);
    // plot lines between points
    mg->Add(gr, "LP");
    graphs.push_back(gr);
  }
  c1->SetLogx();
  c1->SetLogy();
  mg->SetTitle("Hadronic mCP Estimate");
  mg->GetXaxis()->SetTitle("mCP mass (GeV)");
  std::ostringstream datastrs;
  datastrs << data;
  mg->GetYaxis()->SetTitle(
      TString("mCP incident on milliQan (" + datastrs.str() + " fb^{-1})"));
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();
  mg->GetXaxis()->SetTitleOffset(1.2);
  // draw with axes
  mg->Draw("A");
  // add legend to show the pT cuts
  TLegend *legend = new TLegend(0.65, 0.68, .85, .85);
  legend->SetTextSize(0.025);
  for (std::size_t i = 0; i < graphs.size(); i++) {
    Double_t pTcut = pTcuts[i];
    std::ostringstream pTstrs;
    pTstrs << pTcut;
    // calculate approximate charge from pT cut
    Double_t q = calc_q(pTcut);
    std::ostringstream qstrs;
    // only show most significant figure
    int myprecision;
    if (i == 0)
      myprecision = 4;
    else if (i == 4)
      myprecision = 2;
    else
      myprecision = 3;
    qstrs << fixed << setprecision(myprecision) << q;
    legend->AddEntry(graphs[i],
                     TString(pTstrs.str() + " GeV (q=" + qstrs.str() + ")"));
  }
  legend->SetHeader("pT cuts");
  legend->Draw();
  c1->SaveAs("plot.pdf");
}
