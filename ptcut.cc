#include <iostream>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>

void ptcut(Double_t pTcut=0.0) {
  // read in output file
  TFile *EventFile = new TFile("out.root", "READ");
  TTree *sourceTree = (TTree *)EventFile->Get("EventTree");
  unsigned int nentries = sourceTree->GetEntries();
  Double_t pT = 0.0;
  sourceTree->SetBranchAddress("pT", &pT);
  // count how many muons pass a pT cut
  int num_passed_cut = 0;
  for (unsigned int i = 0; i < nentries; i++) {
    sourceTree->GetEntry(i);
    if (pT>pTcut) num_passed_cut++;
  }
  cout << num_passed_cut << " muons passed pT cut of " << pTcut << endl;
}
