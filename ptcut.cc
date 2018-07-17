#include <TFile.h>
#include <TMath.h>
#include <TROOT.h>
#include <TTree.h>
#include <iostream>

void ptcut(Double_t pTcut = 0.0, TString infile = "out.root") {
  // read in input file
  TFile *EventFile = new TFile(infile, "READ");
  TTree *sourceTree = (TTree *)EventFile->Get("EventTree");
  unsigned int nentries = sourceTree->GetEntries();
  Double_t pT = 0.0;
  Double_t weight = 0.0;
  sourceTree->SetBranchAddress("pT", &pT);
  sourceTree->SetBranchAddress("weight", &weight);
  // count how many mCP pass a pT cut (in GeV)
  double event_sum = 0.0;
  double event_sumsq = 0.0;
  for (unsigned int i = 0; i < nentries; i++) {
    sourceTree->GetEntry(i);
    if (pT > pTcut) {
      event_sum += weight;
      event_sumsq += weight * weight;
    }
  }
  // Get event weight that includes cross section and number of events. Cross
  // section is in mb.
  double tree_weight = sourceTree->GetWeight();
  // adjust weight to calculate mCP seen
  tree_weight *= 1e-3;         // mb to b
  tree_weight *= 1e15;         // b to fb
  tree_weight *= 1e-3 * 1e-3;  // multiply by millicharge^2
  tree_weight *= 300.0;        // 300 fb^-1
  tree_weight *= 1e-4;         // milliQan acceptance
  // calculate mCP/event that pass the pT cut
  cout << event_sum * event_sum / event_sumsq
       << " equivalent events pass pT cut of " << pTcut << " GeV" << endl;
  cout << event_sum * tree_weight << "+-"
       << TMath::Sqrt(event_sumsq) * tree_weight
       << " mCP seen with 300 fb^-1 of data" << endl;
}
