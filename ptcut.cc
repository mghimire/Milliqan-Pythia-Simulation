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
  sourceTree->SetBranchAddress("pT", &pT);
  // count how many mCP pass a pT cut (in GeV)
  int num_passcut = 0;
  for (unsigned int i = 0; i < nentries; i++) {
    sourceTree->GetEntry(i);
    if (pT >= pTcut) num_passcut++;
  }
  // Get event weight that includes cross section and number of events. Cross
  // section is in mb.
  double weight = sourceTree->GetWeight();
  // adjust weight to calculate mCP seen
  weight *= 1e-3;         // mb to b
  weight *= 1e15;         // b to fb
  weight *= 1e-3 * 1e-3;  // multiply by millicharge^2
  weight *= 300.0;        // 300 fb^-1
  weight *= 1e-4;         // milliQan acceptance
  // calculate mCP/event that pass the pT cut
  cout << num_passcut * weight << "+-" << TMath::Sqrt(num_passcut) * weight
       << " mCP seen with 300 fb^-1 of data and pT cut of " << pTcut << " GeV"
       << endl;
}
