int EperB(Double_t mean){
//function to calculate the number of signal events per background event required for 95% confidence	
	TF1* f = new TF1("f", "TMath::Poisson(x,[0])",0.,10000.);
	f->SetParameter(0,mean);
	estimate = mean + 2*TMath::Sqrt(mean);
	Double_t integral = 1.0;
	while (integral > 0.05) {
		integral = f->Integral(estimate,10000.);
		estimate++;
	}
	int N = floor(estimate - mean);
	cout << N << endl;
	return N;
}

void limitplot(bool doqcd=0,int bkg=0,TString extra="",int type=2, bool rock=1, Double_t minNPE=0., TString dirname="plots"){
//type mode is for pT cut type (0 for none, 1 for flat, and 2 for triangular) and rock mode is for rock damping cut (0 for off, 1 for on) 
//extra mode is for extensions (suffix) to data directories
	TString typerock = TString("type")+Form("%d",type)+TString("_rock")+Form("%d",rock);
	Double_t timerun = 1088; //in hours

	if (doqcd) cout<<"Adding qcd"<<endl;
	else cout<<"Not adding qcd"<<endl;
	
	TH2D* mCPseengammaZ = (TH2D*)gROOT->Get("mCPseengammaZ"+extra);
	TH2D* mCPseenerrgammaZ = (TH2D*)gROOT->Get("mCPseenerrgammaZ"+extra);
	TH2D* mCPseenonia = (TH2D*)gROOT->Get("mCPseenonia"+extra);
	TH2D* mCPseenerronia = (TH2D*)gROOT->Get("mCPseenerronia"+extra);
	TH2D* mCPseenqcd = (TH2D*)gROOT->Get("mCPseenqcd"+extra);
	TH2D* mCPseenerrqcd = (TH2D*)gROOT->Get("mCPseenerrqcd"+extra);
	TH2D* heff = (TH2D*)gROOT->Get("heff");
	TH1D* background = (TH1D*)gROOT->Get("A__1");
	TH1* integrated = (TH1*)A__1->GetCumulative(kFALSE);
	integrated->Scale(timerun);


	TCanvas *c2 = new TCanvas("c2","mCP_canvas",0,0,600,400);	
	mCPseengammaZ->Draw();
	mCPseenonia->Draw();
	if (doqcd) mCPseenqcd->Draw();
	mCPseengammaZ->Add(mCPseenonia);
	if (doqcd) mCPseengammaZ->Add(mCPseenqcd);
	mCPseengammaZ->Multiply(heff);//account for detector efficiency, loaded in eff.C	
	mCPseengammaZ->SetTitle("mCP Detected vs. Mass and Charge");
	mCPseengammaZ->SetName("mCPseen"+extra+typerock);
	mCPseengammaZ->Draw("colz");
	c2->SetLogz(1);
	
	TString quali="";
	if (doqcd) quali="_qcd"; else quali="_noqcd";
	if (bkg==0) quali+="_dembkg"; else quali+="_origbkg";
	dirname = dirname+"/"+TString(heff->GetTitle()).ReplaceAll(".","p");
	gSystem->Exec("mkdir -p "+dirname);
	c2->SaveAs(dirname+"/effheatplotpTweight_"+typerock+"_"+extra+quali+".C");
   	c2->SaveAs(dirname+"/effheatplotpTweight_"+typerock+"_"+extra+quali+".pdf");
    	
	TCanvas *c3 = new TCanvas("c3","mCP_canvas",0,0,600,400);	
	mCPseenerrgammaZ->Draw();
	mCPseenerronia->Draw();
	if (doqcd) mCPseenerrqcd->Draw();
	mCPseenerrgammaZ->Add(mCPseenerronia);
	if (doqcd) mCPseenerrgammaZ->Add(mCPseenerrqcd);
	mCPseenerrgammaZ->SetTitle("Uncertainty on mCP Detected vs. Mass and Charge");
	mCPseenerrgammaZ->Draw("colz");
	c3->SetLogz(1);
	c3->SaveAs(dirname+"/effheatplotpTweighterr_"+typerock+"_"+extra+quali+".C");
   	c3->SaveAs(dirname+"/effheatplotpTweighterr_"+typerock+"_"+extra+quali+".pdf");

    // Now make contour plot version
	
	
	mCPseengammaZ->SetContour(3);
	if (bkg==0){ // demonstrator data
		Double_t mean = integrated->GetBinContent(integrated->FindBin(minNPE+0.000001));
		int N_3000 = EperB(mean*67*2);
		int N_300  = EperB(mean*67);
		int N_30   = EperB(mean);
		mCPseengammaZ->SetContourLevel(0,N_3000/10.);//6 in 3000/fb, for 2 bkg
		//mCPseengammaZ->SetContourLevel(0,1.8);//18 in 3000/fb, for 10 bkg
		mCPseengammaZ->SetContourLevel(1,N_300*1.);// 4 in 300/fb, for 1 bkg
		mCPseengammaZ->SetContourLevel(2,N_30*667.);// 4 in 30/fb, but 1.5% acceptance, so 4e3 in 300/fb, for 1 bkg
	}
	else if (bkg==1){ // original Gabriel (?)
		mCPseengammaZ->SetContourLevel(0,33.5);// 335 in 3000/fb, for 300 bkg
		mCPseengammaZ->SetContourLevel(1,175.0);// 175 in 300/fb, for 150 bkg
		mCPseengammaZ->SetContourLevel(2,5.0e3);// 5 in 30/fb, but 1% acceptance, so 5e3 in 300/fb, for 150/100=1.5 bkg	
	}
	else cout<<"dont know bkg "<<bkg<<endl;

	TCanvas *c4 = new TCanvas("c3","mCP_canvas",0,0,600,400);	
	mCPseengammaZ->SetTitle("milliQan Sensitivity vs. Mass and Charge at minNPE = "+TString(Form("%.1f",minNPE)));
	mCPseengammaZ->Draw("cont1");
	mCPseengammaZ->SetLineWidth(2);	
	gROOT->ProcessLine(".x oldlimits.cc");
	
	
	c4->SaveAs(dirname+"/limitplotpTweight_"+typerock+"_"+extra+quali+".C");
        c4->SaveAs(dirname+"/limitplotpTweight_"+typerock+"_"+extra+quali+".pdf");
}
