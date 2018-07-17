#include <TFile.h>
#include <TMath.h>
#include <TROOT.h>
#include <TTree.h>
#include <iostream>

// calculate pseudorapidity from theta
Double_t calc_eta(Double_t theta) {
  return -1.0 * TMath::Log(TMath::Tan(theta / 2));
}

void ptcut(Double_t pTcut = 0.0, TString infile = "out.root") {
  // calculation parameters
  Double_t charge = 1e-3;  // charge in e
  Double_t data = 300.0;   // 300 fb^-1

  // calculate eta/phi acceptance
  Double_t det_loc = 43.1 * TMath::Pi() / 180.0;  // detector at 43.1 deg
  Double_t det_width = 1.0;                       // detector is 1x1 m
  Double_t det_dis = 33.0;                        // detector dist 33 m
  Double_t angle_sub = det_width / det_dis;       // angle subtended by detector
  Double_t phi_acceptance = angle_sub / (2 * TMath::Pi());  // phi acceptance
  // calculate eta with width extra_width times larger for good stats
  Double_t extra_width = 10.0;
  Double_t low_eta = calc_eta(det_loc + 0.5 * angle_sub * extra_width);
  Double_t high_eta = calc_eta(det_loc - 0.5 * angle_sub * extra_width);

  // read in input file
  TFile *EventFile = new TFile(infile, "READ");
  TTree *sourceTree = (TTree *)EventFile->Get("EventTree");
  unsigned int nentries = sourceTree->GetEntries();
  Double_t pT = 0.0;
  Double_t weight = 0.0;
  Double_t eta = 0.0;
  sourceTree->SetBranchAddress("pT", &pT);
  sourceTree->SetBranchAddress("eta", &eta);
  sourceTree->SetBranchAddress("weight", &weight);
  // count how many mCP pass a pT cut (in GeV)
  double event_sum = 0.0;
  double event_sumsq = 0.0;
  for (unsigned int i = 0; i < nentries; i++) {
    sourceTree->GetEntry(i);
    Double_t abs_eta = TMath::Abs(eta);
    if (pT > pTcut && abs_eta > low_eta && abs_eta < high_eta) {
      event_sum += weight;
      event_sumsq += weight * weight;
    }
  }
  // Get event weight that includes cross section and number of events. Cross
  // section is in mb.
  double tree_weight = sourceTree->GetWeight();
  // adjust weight to calculate mCP seen
  tree_weight *= 1e-3;               // mb to b
  tree_weight *= 1e15;               // b to fb
  tree_weight *= data;               // data in fb^-1
  tree_weight *= charge * charge;    // multiply by millicharge^2
  tree_weight *= phi_acceptance;     // phi acceptance
  tree_weight *= 1.0 / extra_width;  // adjust for large eta width
  tree_weight *= 0.5;                // adjust for having both + and - eta

  // calculate mCP/event that pass the pT cut
  cout << event_sum * event_sum / event_sumsq
       << " equivalent events pass pT cut of " << pTcut << " GeV" << endl;
  cout << event_sum * tree_weight << "+-"
       << TMath::Sqrt(event_sumsq) * tree_weight << " mCP seen with " << data
       << " fb^-1 of data" << endl;
}
