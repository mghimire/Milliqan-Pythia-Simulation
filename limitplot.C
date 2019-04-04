void limitplot(bool doqcd=0){
	mCPseengammaZ->Draw();
	mCPseenonia->Draw();
	if (doqcd) mCPseenqcd->Draw();
	mCPseengammaZ->Add(mCPseenonia);
	if (doqcd) mCPseengammaZ->Add(mCPseenqcd);
	mCPseengammaZ->SetTitle("mCP Seen vs. Mass and Charge");
	mCPseengammaZ->Draw("colz");
	//mCP_canvas->SetLogz(1);
	
	mCPseengammaZ->SetContour(4);
	mCPseengammaZ->SetContourLevel(0,1.8);//18 in 3000/fb
	mCPseengammaZ->SetContourLevel(1,4.0);// 4 in 300/fb
	mCPseengammaZ->SetContourLevel(2,10.0);// 1 in 30/fb
	mCPseengammaZ->SetContourLevel(3,1.0e3);// 1 in 30/fb or 10 in 300/fb, but 1% acceptance, so 1e3 in 300/fb
	//mCPseengammaZ->Draw("colz");
	mCPseengammaZ->Draw("cont1");
}
