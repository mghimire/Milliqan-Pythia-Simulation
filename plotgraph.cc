#include <iostream>
using std::cin;
#include <fstream>
using std::ifstream;
#include <iterator>
using std::istream_iterator;
#include <algorithm>
using std::copy;
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
#include <fstream>
#include <iterator>
#include <algorithm>

// load ptcut.cc macro to do analysis as mCP_anal struct defined there

void plotgraph() {
  // load up the filenames of files in data/ to a vector of TStrings
  std::vector<TString> filenames;
  ifstream myfile("filenames.txt"); 

  copy(istream_iterator<TString>(myfile),
       istream_iterator<TString>(),
       back_inserter(filenames));

  //create a vector of masses being used for future reference
  std::vector<Double_t> masses;
  for (std::size_t i = 0; i < filenames.size(); i++) {
    if (filenames[i].EndsWith("_1.root")) masses.push_back(filenames[i].Atof());
  }

  //create a vector of vectors per mass of filenames
  std::vector<vector<TString>> files;
  int f_i = 0;
  do {
      std::vector<TString> massfiles;
      TString check = filenames[f_i](0,filenames[f_i].First("_"));
      do {
          massfiles.push_back("data/" + filenames[f_i]);
          f_i++;
      } while (filenames[f_i].BeginsWith(check));
      files.push_back(massfiles);
  } while (f_i < filenames.size());

  // vector of charges we will plot
  std::vector<Double_t> charges; 
//  charges = {0.001, 0.005, 0.01, 0.05, 0.1, 0.3};
  for (int i = 0; i < 11; i++) {
    charges.push_back(0.0001*TMath::Power(10, (4*i / 10.0)));
  }
  // vector of analyses of those charges
  std::vector<std::vector<mCP_anal>> q_analyses;

  // do analysis with each charge and add to master list
  for (std::size_t q_i = 0; q_i < charges.size(); q_i++) {
    // run analysis on each mass size with given charge
    Double_t q = charges[q_i];
    std::vector<mCP_anal> analyses;
    for (std::size_t i = 0; i < files.size(); i++) {
      mCP_anal analysis = analyze_pythia_sim(q, files[i]);
      analyses.push_back(analysis);
    }
    q_analyses.push_back(analyses);
  }

  // output amount of fb^-1 calculation is based on
  cout << "data: " << data << " fb^-1" << endl;
  cout << endl;

  // output table of analysis result points and acceptance for each pT cut and
  // mass
  for (std::size_t q_i = 0; q_i < charges.size(); q_i++) {
    std::vector<mCP_anal> analyses = q_analyses[q_i];
    Double_t charge = charges[q_i];
    // output mCP charge used
    cout << "Next table mCP charge: " << charge << "e" << endl;
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

  // plot graph of charge vs mass with 
  TCanvas *c1 = new TCanvas("c","mCP_canvas",0,0,600,400);
  
//  c1->SetLogx(1);
//  c1->SetLogy(1);
  c1->SetLogz(1);

  TH2D *g = new TH2D("mCPseen", "mCP seen vs Mass vs Charge; log10 of Mass (GeV); log10 of Charge (e); Number of Particles Seen", 12, -2, 1, 9, -4, 0.5);

  for (std::size_t m_i = 0; m_i < masses.size(); m_i++) {
    for (std::size_t q_i = 0; q_i < charges.size(); q_i++) {
      if (q_analyses[q_i][m_i].mCP_seen != 0) //option to keep or reject z = 0 points
	g->Fill(std::log10(masses[m_i]), std::log10(charges[q_i]), q_analyses[q_i][m_i].mCP_seen);
    }
  }
  g->Draw("colz");
  c1->SaveAs("heatplotpTweight.C");
  c1->SaveAs("heatplotpTweight.pdf");
/*  std::vector<TGraph *> graphs;
    for (std::size_t q_i = 0; q_i < charges.size(); 1_i++) {
    std::vector<mCP_anal> analyses = q_analyses[q_i];
    Double_t charge = charges[q_i];
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
                     TString(pTstrs.str() + " GeV (q=" + qstrs.str() + "e)"));
  }
  legend->SetHeader("pT cuts");
  legend->Draw();
  c1->SaveAs("plot.pdf");
*/

}
