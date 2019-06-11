void SLPeff(TString SLP="0", Double_t minNPE=0., TString dirname="plots"){
	
	TString filename = TString("outputSignalEffGraph.root");
	TString graphname = TString("minNPE")+Form("%.1f",minNPE);
	TString pathname = TString("pathTuple")+SLP+TString("/")+graphname;
	graphname = graphname.ReplaceAll(".","p");
	TString effplotname = TString("SLP") + SLP + "_" + graphname;
	TString title = TString("pathTuple")+SLP+TString("/")+graphname;

	TFile *SLPFile = new TFile(filename, "READ");
    	TGraph *SLPGraph = (TGraph *)SLPFile->Get(pathname);

	TCanvas *c2 = new TCanvas("c2", "Canvas_1_n2",503,358,538,327);
    	c2->Range(-0.006850001,-0.1375,0.06165,1.2375);
    	c2->SetFillColor(0);
    	c2->SetBorderMode(0);
    	c2->SetBorderSize(2);
    	c2->SetFrameBorderMode(0);
    	c2->SetFrameBorderMode(0);
	
	SLPGraph->SetMarkerStyle(5);
	SLPGraph->SetTitle(pathname);
    	SLPGraph->Draw("alp");
    	c2->Modified();
    	c2->cd();
    	c2->SetSelected(c2);
	
	Double_t parest = 0.0;
	if (SLP=="All"){
		parest = 0.45;
	} else parest = 0.14/2;

	TF1 *fit = new TF1("fit", "[0]*(TMath::Erf([1]*x+[2])+1)",0.0,1.0);
   	fit->SetParameter(0,parest);
   	SLPGraph->Fit("fit", "R");

	SLPGraph->Draw("alp");
   	c2->Update();
   	c2->Modified();

	TLine *opt = new TLine(0.0025,0.,0.0025,1.);
	opt->Draw("alp");
	c2->Update();
   	c2->Modified();

	dirname = dirname+"/"+title;
    	gSystem->Exec("mkdir -p "+dirname);
	c2->SaveAs(dirname+"/"+effplotname+"fit.C");//unity efficiency
    	c2->SaveAs(dirname+"/"+effplotname+"fit.pdf");

	TH2D* heff = new TH2D("heff","heff", 41, -2, 2.3, 42, -4, 0.01);
	
	heff->SetTitle(pathname);
	
	Int_t nbinsx = heff->GetNbinsX();
    	Int_t nbinsy = heff->GetNbinsY();

    	for (int biny=1;biny<=nbinsy;biny++) {
       	  for (int binx=1;binx<=nbinsx;binx++) {
            Double_t x = heff->GetXaxis()->GetBinCenter(binx);
	    Double_t y = heff->GetYaxis()->GetBinCenter(biny);
	    Double_t q = pow(10,y);
	    Double_t w = fit->Eval(q);
	    heff->Fill(x,y,w);
          }
        }
	if (SLP!="All") heff->Scale(6.);
    
    	TCanvas *c1 = new TCanvas("c1","mCP_canvas",0,0,600,400);
    	heff->SetStats(0);
    	heff->Draw("colz");
    	c1->SaveAs(dirname+"/"+effplotname+".C");//unity efficiency
    	c1->SaveAs(dirname+"/"+effplotname+".pdf");
}
