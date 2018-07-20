#include <TFile.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TROOT.h>
#include <TTree.h>
#include <TVectorD.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include "ptcut.cc"

// load ptcut.cc macro to do analysis as mCP_anal struct defined there
void plotgraph(Double_t pTcut = 1.0) {
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
  num_files.push_back(0);

  masses.push_back("0.329");
  num_files.push_back(0);

  masses.push_back("0.5");
  num_files.push_back(0);

  masses.push_back("1");
  num_files.push_back(0);

  masses.push_back("2");
  num_files.push_back(0);

  masses.push_back("10");
  num_files.push_back(0);

  std::vector<vector<TString>> files;
  // create lists of file names
  for (std::size_t i = 0; i < masses.size(); i++) {
    std::vector<TString> mass_files;
    for (int j = 1; j <= num_files[i]; j++) {
      mass_files.push_back("data/" + masses[i] + "_" + j + ".root");
    }
    if (mass_files.size() > 0) files.push_back(mass_files);
  }

  // run analysis on each mass size
  std::vector<mCP_anal> analyses;
  for (std::size_t i = 0; i < files.size(); i++) {
    mCP_anal analysis = analyze_pythia_sim(pTcut, files[i]);
    analyses.push_back(analysis);
  }

  // output mCP pT cut used
  cout << "mCP pT cut: " << pTcut << " GeV" << endl;

  // output table of analysis results
  cout << std::setfill('-') << std::setw(33) << "" << std::setfill(' ') << endl;
  cout << std::setw(7) << "m (GeV)"
       << " " << std::setw(11) << "mCP seen"
       << "   " << std::setw(11) << "acceptance" << endl;
  cout << std::setfill('-') << std::setw(33) << "" << std::setfill(' ') << endl;
  for (std::size_t i = 0; i < analyses.size(); i++) {
    mCP_anal anal = analyses[i];
    std::ostringstream mCP_seen_err;
    std::ostringstream acceptance_err;
    mCP_seen_err << "+-" << anal.mCP_seen_err;
    acceptance_err << "+-" << anal.acceptance_err;
    cout << std::setw(5) << anal.mass << "   " << std::setw(11) << anal.mCP_seen
         << "   " << std::setw(11) << anal.acceptance << endl;
    cout << std::setw(5) << " "
         << " " << std::setw(13) << mCP_seen_err.str() << " " << std::setw(13)
         << acceptance_err.str() << endl;
  }
  cout << std::setfill('-') << std::setw(33) << "" << std::setfill(' ') << endl;

  // plot graph of mCP incident on milliQan
  TCanvas *c1 = new TCanvas(TString("mCP_canvas"));
  c1->SetLogx();
  c1->SetLogy();

  std::vector<Double_t> x;
  std::vector<Double_t> y;
  std::vector<Double_t> ex;
  std::vector<Double_t> ey;
  for (std::size_t i = 0; i < analyses.size(); i++) {
    mCP_anal anal = analyses[i];
    x.push_back(anal.mass);
    ex.push_back(0.0);
    y.push_back(anal.mCP_seen);
    ey.push_back(anal.mCP_seen_err);
  }
  Int_t n = x.size();

  std::ostringstream pTstrs;
  pTstrs << pTcut;

  TGraphErrors *gr =
      new TGraphErrors(n, x.data(), y.data(), ex.data(), ey.data());
  gr->SetTitle(
      TString("Hadronic mCP Estimate (" + pTstrs.str() + " GeV mCP pT cut)"));
  gr->GetXaxis()->SetTitle("mCP mass (GeV)");
  gr->GetYaxis()->SetTitle("mCP incident on milliQan");
  gr->Draw("ALP");
  c1->SaveAs("plot.pdf");
}
