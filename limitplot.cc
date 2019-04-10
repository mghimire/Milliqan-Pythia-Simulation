void limitplot(bool doqcd=0,int bkg=0,TString extra=""){
	if (doqcd) cout<<"Adding qcd"<<endl;
	else cout<<"Not adding qcd"<<endl;
	
	TH2D* mCPseengammaZ = (TH2D*)gROOT->Get("mCPseengammaZ"+extra);
	TH2D* mCPseenonia = (TH2D*)gROOT->Get("mCPseenonia"+extra);
	TH2D* mCPseenqcd = (TH2D*)gROOT->Get("mCPseenqcd"+extra);
	TH2D* heff = (TH2D*)gROOT->Get("heff");
	
	TCanvas *c2 = new TCanvas("c2","mCP_canvas",0,0,600,400);	
	mCPseengammaZ->Draw();
	mCPseenonia->Draw();
	if (doqcd) mCPseenqcd->Draw();
	mCPseengammaZ->Add(mCPseenonia);
	if (doqcd) mCPseengammaZ->Add(mCPseenqcd);	
	mCPseengammaZ->Multiply(heff);//account for detector efficiency, loaded in eff.C	
	mCPseengammaZ->SetTitle("mCP Detected vs. Mass and Charge");
	mCPseengammaZ->Draw("colz");
	c2->SetLogz(1);
	
	TString quali="";
	if (doqcd) quali="_qcd"; else quali="_noqcd";
	if (bkg==0) quali+="_optbkg"; else quali+="_origbkg";
	gSystem->Exec("mkdir plots");
	c2->SaveAs("plots/effheatplotpTweight"+extra+quali+"_"+heff->GetTitle()+".C");
   	c2->SaveAs("plots/effheatplotpTweight"+extra+quali+"_"+heff->GetTitle()+".pdf");
    
    // Now make contour plot version	
	mCPseengammaZ->SetContour(3);	
	if (bkg==0){ // optimistic 
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
	else cout<<"dont know bkg "<<bkg<<endl;

	TCanvas *c3 = new TCanvas("c3","mCP_canvas",0,0,600,400);	
	mCPseengammaZ->SetTitle("milliQan Sensitivity vs. Mass and Charge");
	mCPseengammaZ->Draw("cont1");
	mCPseengammaZ->SetLineWidth(2);	
	gROOT->ProcessLine(".x oldlimits.cc");
	
	c3->SaveAs("plots/limitplotpTweight"+extra+quali+"_"+heff->GetTitle()+".C");
    c3->SaveAs("plots/limitplotpTweight"+extra+quali+"_"+heff->GetTitle()+".pdf");
}
