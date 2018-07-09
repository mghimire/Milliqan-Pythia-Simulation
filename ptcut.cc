#include <TFile.h>
#include <TROOT.h>
#include <TTree.h>
#include <iostream>

void ptcut(Double_t pTcut = 0.0, TString infile = "out.root") {
  // read in output file
  TFile *EventFile = new TFile(infile, "READ");
  TTree *sourceTree = (TTree *)EventFile->Get("EventTree");
  unsigned int nentries = sourceTree->GetEntries();
  Double_t pT = 0.0;
  sourceTree->SetBranchAddress("pT", &pT);
  // count how many muons pass a pT cut (in GeV)
  int num_passed_cut = 0;
  for (unsigned int i = 0; i < nentries; i++) {
    sourceTree->GetEntry(i);
    if (pT >= pTcut) num_passed_cut++;
  }
  cout << num_passed_cut << " muons passed pT cut of " << pTcut << endl;
}
