#include <TFile.h>
#include <TMath.h>
#include <TROOT.h>
#include <TTree.h>
#include <TVectorD.h>
#include <iostream>
#include <vector>
#include "ptcut.cc"

// load ptcut.cc macro to do analysis as mCP_anal struct defined there
void plotgraph(Double_t pTcut = 0.0) {
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
    files.push_back(mass_files);
  }

  // run analysis on each file
  std::vector<mCP_anal> analyses;
  for (std::size_t i = 0; i < files.size(); i++) {
    mCP_anal analysis = analyze_pythia_sim(pTcut, files[i]);
    analyses.push_back(analysis);
  }
}
