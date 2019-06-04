void SLPeff(TString SLP="0", TString dirname="plots", Double_t minNPE=0.){
	
	TString filename = TString("outputSignalEffGraph.root");
	TString graphname = TString("minNPE")+Form("%.1f",minNPE);
	TString pathname = TString("pathTuple")+SLP+TString("/")+graphname;
	graphname = graphname.ReplaceAll(".","p");
	TString title = TString("SLP") + SLP + "_" + graphname;

	TFile *SLPFile = new TFile(filename, "READ");
    	TGraph *SLPGraph = (TGraph *)SLPFile->Get(pathname);

	TF1 *fit = new TF1("fit", "[0]*(TMath::Erf([1]*x+[2])+1)",0.0,1.0);
   	fit->SetParameter(0,1);
   	SLPGraph->Fit("fit", "R");

	TFormula *SLPfit = (TFormula *)fit->GetFormula();

	TH2D* heff = new TH2D("heff","heff", 41, -2, 2.3, 42, -4, 0.01);
	
	heff->SetTitle(title);
	
	Int_t nbinsx = heff->GetNbinsX();
    	Int_t nbinsy = heff->GetNbinsY();

	SLPfit->Print();

    	for (int biny=1;biny<=nbinsy;biny++) {
       	  for (int binx=1;binx<=nbinsx;binx++) {
            Double_t x = heff->GetXaxis()->GetBinCenter(binx);
	    Double_t y = heff->GetYaxis()->GetBinCenter(biny);
	    Double_t q = pow(10,y);
	    Double_t w = SLPfit->Eval(q);
	    heff->Fill(x,y,w);
          }
        }
    
    TCanvas *c1 = new TCanvas("c1","mCP_canvas",0,0,600,400);
    heff->SetStats(0);
    heff->Draw("colz");
    gSystem->Exec("mkdir -p "+dirname);
    c1->SaveAs(dirname+"/"+title+".C");//unity efficiency
    c1->SaveAs(dirname+"/"+title+".pdf");	
}
