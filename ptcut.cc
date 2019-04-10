#include <TFile.h>
#include <TMath.h>
#include <TROOT.h>
#include <TTree.h>
#include <TVectorD.h>
#include <TCanvas.h>
#include <TRandom.h>
#include <TDirectory.h>
#include <iostream>
#include <vector>

// struct of data that gets analyzed
typedef struct {
  Double_t mass;            // mCP mass
  Double_t equiv_events;    // equivalent # of unweighted events
  Double_t acceptance;      // milliQan acceptance
  Double_t acceptance_err;  // acceptance uncertainty
  Double_t mCP_seen;        // number of mCP seen by milliQan
  Double_t mCP_seen_err;    // uncertainty in mCP seen
} mCP_anal;

// calculation parameters
Double_t data = 300.0;  // 300 fb^-1

// calculate pseudorapidity from theta
Double_t calc_eta(Double_t theta) {
  return -1.0 * TMath::Log(TMath::Tan(theta / 2.0));
}

//define global variables and resulting values outside
Double_t det_length = 3.0;
Double_t width_tolerance = 0.05;// *2;//allow for wider paths?
Double_t Bfield = 3.8;
Double_t Bfield_R = 3.0;
Double_t Theta = TMath::ATan(width_tolerance/det_length);
Double_t pTtoQ_Conv_factor = 3.3 * TMath::Sin(22 * Theta) / (Bfield_R * Bfield);

// calculate pT cut from charge based on approximate angle coming out from
// detector
Double_t calc_pT(Double_t q) {
  return q / pTtoQ_Conv_factor;
}

mCP_anal analyze_pythia_sim(Double_t charge = 1e-3,
                            std::vector<TString> infiles = {"out.root"}, bool hist = 0) {
  mCP_anal analysis;

  Double_t pTcut = calc_pT(charge);
  //cout<<"ptcut is "<<pTcut<<endl;
  
  // calculate eta/phi acceptance
  Double_t det_loc = 84.0 * TMath::Pi() / 180.0;  // detector at th=84 deg 
  Double_t det_width = 1.0;                    // detector is 1x1 m
  Double_t det_dis = 33.0;                     // detector dist 33 m
  Double_t angle_sub = det_width / det_dis;    // angle subtended by detector
  Double_t phi_acceptance = angle_sub / (2 * TMath::Pi());  // phi acceptance
  // calculate eta with width extra_width times larger for good stats
  Double_t low_eta = 0.0;
  Double_t high_eta = 2.0;
  Double_t actual_eta_width = calc_eta(det_loc - 0.5*angle_sub) - calc_eta(det_loc + 0.5*angle_sub);

  // initialize variables
  double event_sum = 0.0;
  double event_sumsq = 0.0;
  double sum_noetacut = 0.0;
  double sum_noetacutsq = 0.0;
  double sum_invtree_weight = 0.0;
  double sum_invtree_weight_err_sq = 0.0;

  TH1D* mother_id_hist;
  TH1D* pT_hist;
  TH1D* pTHat_hist;
  TH1D* eta_hist;
  if (hist){
	mother_id_hist = new TH1D("mother_id_hist", "mother_id", 200, 0, 200000);
	pT_hist = new TH1D("pT_hist", "pT", 100, 0, 2);
	pTHat_hist = new TH1D("pTHat_hist", "pTHat", 100, 0, 20);
	eta_hist = new TH1D("eta_hist", "eta", 100, -15, 15);
  }

  // loop through input root files
  for (std::size_t f = 0; f < infiles.size(); f++) {
    // read in input file
    TFile *EventFile = new TFile(infiles[f], "READ");
    TTree *sourceTree = (TTree *)EventFile->Get("EventTree");
    unsigned int nentries = sourceTree->GetEntries();
    // Get event tree weight that includes cross section and number of events.
    // Cross section is in mb. Also gets its uncertainty (due to cross section
    // uncertainty output by Pythia).
    Double_t tree_weight = sourceTree->GetWeight();
    TVectorD *tree_err_vec = (TVectorD *)sourceTree->GetUserInfo()->At(0);
    Double_t tree_w_err = tree_err_vec[0][0];
    // also set mCP mass
    analysis.mass = tree_err_vec[0][1];

    sum_invtree_weight += 1.0 / tree_weight;
    //cout << "tree weight is " << tree_weight << endl;
    sum_invtree_weight_err_sq +=
        TMath::Power(tree_w_err / (tree_weight * tree_weight), 2);
    
    Double_t pT = 0.0;
    Double_t weight = 0.0;
    Double_t eta = 0.0;
    Int_t mother_id = 0;
    Double_t pTHat = 0.0;

    sourceTree->SetBranchAddress("pT", &pT);
    sourceTree->SetBranchAddress("eta", &eta);
    sourceTree->SetBranchAddress("weight", &weight);
    //additional data for creating histogram
    if (hist) {
      sourceTree->SetBranchAddress("mother_id", &mother_id);
      sourceTree->SetBranchAddress("pTHat", &pTHat);
    }
    // count how many weighted mCP pass a pT cut (in GeV) and propagate errors
    for (unsigned int i = 0; i < nentries; i++) {
      sourceTree->GetEntry(i);
      Double_t abs_eta = TMath::Abs(eta);
      TRandom r;
      //check if it passes triangular PDF
      if (r.Uniform() <= 1.0 - pTcut/pT) {
		if (pT < 16. * charge * charge) {// check that we have enough pT to get through the rock
			//cout<<"stuck in rock for charge "<<charge<<endl;
			continue;
		}
		if (hist){
			eta_hist->Fill(eta,weight);
		}
        // check if they pass eta cut for milliqan
        if (abs_eta > low_eta && abs_eta < high_eta) {
          event_sum += weight;
          event_sumsq += weight * weight;
			if (hist){
				mother_id_hist->Fill(mother_id,weight);
				pT_hist->Fill(pT,weight);
				pTHat_hist->Fill(pTHat,weight);
			}
        }
        sum_noetacut += weight;
        sum_noetacutsq += weight * weight;
      }
    }
    EventFile->Close();
  }
  if (hist){
    TCanvas* c = new TCanvas("c", "mPC histograms",0,0,800,800);
    c->SetLogy();
    c->Divide(2,2);
    c->cd(1);
    mother_id_hist->Draw("bar0");
    c->cd(2);
    pT_hist->Draw("bar0");
    c->cd(3);
    pTHat_hist->Draw("bar0");
    c->cd(4);
    eta_hist->Draw("bar0");
    c->cd();
    TString dirname = TString("hists_eta")+Form("%f",low_eta)+TString("-")+Form("%f",high_eta);
    gSystem->Exec("mkdir "+dirname);
    TString out = dirname  + TString("/hist_") + Form("%f",analysis.mass) + TString("GeV_") + Form("%f",charge) + TString("e.pdf");
    c->SaveAs(out);
  }

  Double_t event_sum_error = TMath::Sqrt(event_sumsq);
  Double_t sum_noetacut_error = TMath::Sqrt(sum_noetacutsq);

  // calculate detector acceptance and its uncertainty
  Double_t acceptance_reweight = phi_acceptance * 0.5;
  Double_t acceptance_no_rw = event_sum / sum_noetacut;
  Double_t acceptance_no_rw_error =
      acceptance_no_rw *
      TMath::Sqrt(TMath::Power(event_sum_error / event_sum, 2) +
                  TMath::Power(sum_noetacut_error / sum_noetacut, 2));
  Double_t acceptance = acceptance_no_rw * acceptance_reweight;
  Double_t acceptance_error = acceptance_no_rw_error * acceptance_reweight;

  Double_t sum_invtree_weight_error = TMath::Sqrt(sum_invtree_weight_err_sq);
  Double_t reweight_from_tree = 1.0 / sum_invtree_weight;
  Double_t reweight_from_tree_error =
      sum_invtree_weight_error / (sum_invtree_weight * sum_invtree_weight);

  Double_t event_sum_reweight = event_sum * reweight_from_tree;
  //cout << "event sum is " << event_sum << endl;
  //cout << "reweight from tree is " << reweight_from_tree << endl;
  Double_t event_sum_reweight_error =
      event_sum_reweight *
      TMath::Sqrt(
          TMath::Power(event_sum_error / event_sum, 2) +
          TMath::Power(reweight_from_tree_error / reweight_from_tree, 2));

  // calculate final reweighting for events to calculate mCP seen
  double final_reweight = 1.0;
  final_reweight *= 1e-3;               			// mb to b
  final_reweight *= 1e15;               			// b to fb
  final_reweight *= data;               			// data in fb^-1
  final_reweight *= charge * charge;    			// multiply by millicharge^2
  final_reweight *= phi_acceptance;     			// phi acceptance
  final_reweight *= actual_eta_width / (high_eta - low_eta);  	// adjust for the large eta width
  final_reweight *= 0.5;                			// adjust for having both + and - eta

  // calc equiv num of events stat-wise if each event had weight 1
  analysis.equiv_events = event_sum * event_sum / event_sumsq;
  // set acceptance that was calculated
  analysis.acceptance = acceptance;
  analysis.acceptance_err = acceptance_error;
  // calculate mCP/event that pass the pT cut
  analysis.mCP_seen = event_sum_reweight * final_reweight;
  analysis.mCP_seen_err = event_sum_reweight_error * final_reweight;
  return analysis;
}

void ptcut(Double_t charge, std::vector<TString> infiles = {"out.root"}, bool hist = 0) {
  mCP_anal analysis = analyze_pythia_sim(charge, infiles, hist);
  // output analysis calculated
  cout << "mCP mass is " << analysis.mass << " GeV" << endl;
  cout << analysis.equiv_events << " equivalent events pass pT cut of " << calc_pT(charge)
       << " GeV" << endl;
  cout << "acceptance is: " << analysis.acceptance << "+-"
       << analysis.acceptance_err << endl;
  cout << analysis.mCP_seen << "+-" << analysis.mCP_seen_err
       << " mCP seen with " << data << " fb^-1 of data" << endl;
}
