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
#include <sstream>
#include <vector>
#include "ptcut.cc"

// call like: root -l 'plotgraph.cc("gammaZ")'

// load ptcut.cc macro to do analysis as mCP_anal struct defined there

void plotgraph(TString extra, bool dohist=0) {
  // load up the filenames of files in data/ to a vector of TStrings
  std::vector<TString> filenames;
  ifstream myfile("filenames"+extra+".txt"); 

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
          massfiles.push_back("data"+extra+"/" + filenames[f_i]);
          f_i++;
      } while (filenames[f_i].BeginsWith(check));
      files.push_back(massfiles);
      cout<<"Found "<<massfiles.size()<<" files for "<<check<<endl;
  } while (f_i < filenames.size());

  // vector of charges we will plot
  std::vector<Double_t> charges; 
  int nch=40; // number of charges to try
  for (int i = 0; i <= nch+1; i++) {
    charges.push_back(0.0001*TMath::Power(10, (4*i / (double)(nch+1))));
  }
  // vector of analyses of those charges
  std::vector<std::vector<mCP_anal>> q_analyses;

  // do analysis with each charge and add to master list
  for (std::size_t q_i = 0; q_i < charges.size(); q_i++) {
    // run analysis on each mass size with given charge
    Double_t q = charges[q_i];
    cout<<"charge "<<q<<endl;
    std::vector<mCP_anal> analyses;
    for (std::size_t i = 0; i < files.size(); i++) {
      mCP_anal analysis = analyze_pythia_sim(q, files[i], dohist, extra);
      analyses.push_back(analysis);
    }
    q_analyses.push_back(analyses);
  }

  // output amount of fb^-1 calculation is based on
  cout << "data: " << data << " fb^-1" << endl;

  // output table of analysis result points and acceptance for each pT cut and mass
  /*
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
  */

  // plot graph of charge vs mass with 
  TCanvas *c1 = new TCanvas("c1","mCP_canvas",0,0,600,400);

  gStyle->SetOptStat(0);
  c1->SetLogz(1);

  TH2D *g = new TH2D("mCPseen"+extra, "mCP seen from "+extra+" vs Mass vs Charge; log10 of Mass (GeV); log10 of Charge (e)", 41, -2, 2.3, nch+2, -4, 0.01); // can go to 40 masses, and just 2.3 high
  TH2D *gtest = new TH2D(*g); gtest->SetName("gtest"); // a copy of the hist, to check that each bin is only filled once
  TH2D *gerror = new TH2D("mCPseenerr"+extra, "Uncertainty on mCP seen from "+extra+" vs Mass vs Charge; log10 of Mass (GeV); log10 of Charge (e)", 41, -2, 2.3, nch+2, -4, 0.01); // can go to 40 masses, and just 2.3 high

  for (std::size_t m_i = 0; m_i < masses.size(); m_i++) {
    for (std::size_t q_i = 0; q_i < charges.size(); q_i++) {
	  if (gtest->GetBinContent(gtest->FindBin(std::log10(masses[m_i]), std::log10(charges[q_i])))!=0){
		  cout<<"Bin already filled at "<<masses[m_i]<<" "<<charges[q_i]<<endl;
	  }
      if (q_analyses[q_i][m_i].mCP_seen != 0) {//option to keep or reject z = 0 points
		g->Fill(std::log10(masses[m_i]), std::log10(charges[q_i]), q_analyses[q_i][m_i].mCP_seen);
		gerror->Fill(std::log10(masses[m_i]), std::log10(charges[q_i]), q_analyses[q_i][m_i].mCP_seen_err/q_analyses[q_i][m_i].mCP_seen);
	  }
	  gtest->Fill(std::log10(masses[m_i]), std::log10(charges[q_i]));
    }
  }
  g->Draw("colz");
  gSystem->Exec("mkdir plots");
  c1->SaveAs("plots/heatplotpTweight_"+extra+".C");
  c1->SaveAs("plots/heatplotpTweight_"+extra+".pdf");
  gerror->Draw("colz");
  c1->SaveAs("plots/heatplotpTweighterr_"+extra+".C");
  c1->SaveAs("plots/heatplotpTweighterr_"+extra+".pdf");
}
