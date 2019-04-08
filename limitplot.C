void limitplot(bool doqcd=0,TString extra=""){
	if (doqcd) cout<<"Adding qcd"<<endl;
	else cout<<"Not adding qcd"<<endl;
	mCPseengammaZ->Draw();
	mCPseenonia->Draw();
	if (doqcd) mCPseenqcd->Draw();
	mCPseengammaZ->Add(mCPseenonia);
	if (doqcd) mCPseengammaZ->Add(mCPseenqcd);
	
	mCPseengammaZ->Multiply(h3__1);//account for detector efficiency, loaded in eff.C
	
	mCPseengammaZ->SetTitle("mCP Seen vs. Mass and Charge");
	mCPseengammaZ->Draw("colz");
	//mCP_canvas->SetLogz(1);
	
	mCPseengammaZ->SetContour(3);
	int bkg=0;
	
	if (bkg==0){ // optimiztic 
	mCPseengammaZ->SetContourLevel(0,0.6);//6 in 3000/fb, for 2 bkg
	//mCPseengammaZ->SetContourLevel(0,1.8);//18 in 3000/fb, for 10 bkg
	mCPseengammaZ->SetContourLevel(1,4.0);// 4 in 300/fb, for 1 bkg
	mCPseengammaZ->SetContourLevel(2,4.0e3);// 4 in 30/fb, but 1% acceptance, so 4e3 in 300/fb, for 1 bkg
	}
	else if (bkg==1){ // original Gabriel (?)
	mCPseengammaZ->SetContourLevel(0,33.5);// 335 in 3000/fb, for 300 bkg
	mCPseengammaZ->SetContourLevel(1,175.0);// 175 in 300/fb, for 150 bkg
	mCPseengammaZ->SetContourLevel(2,5.0e3);// 5 in 30/fb, but 1% acceptance, so 5e3 in 300/fb, for 150/100=1.5 bkg	
	}

	TCanvas *c1 = new TCanvas("c1","mCP_canvas",0,0,600,400);
	
	//mCPseengammaZ->Draw("colz");
	mCPseengammaZ->Draw("cont1");
	mCPseengammaZ->SetLineWidth(2);
	
	gROOT->ProcessLine(".x oldlimits.C");
	if (doqcd){
	c1->SaveAs("limitplotpTweightqcd"+extra+".C");
    c1->SaveAs("limitplotpTweightqcd"+extra+".pdf");
	}
	else{
	c1->SaveAs("limitplotpTweight"+extra+".C");
    c1->SaveAs("limitplotpTweight"+extra+".pdf");
	}
}
