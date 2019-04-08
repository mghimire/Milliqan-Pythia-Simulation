void filleff(){

	TH2D* h3 = new TH2D("h3","eff", 20, -2, 3, 40, -4, 0);
	
	Int_t nbinsx = h3->GetNbinsX();
    Int_t nbinsy = h3->GetNbinsY();
	int nlay=4; // number of layers
    
    for (int biny=1;biny<=nbinsy;biny++) {
       for (int binx=1;binx<=nbinsx;binx++) {
          
        double x    = h3->GetXaxis()->GetBinCenter(binx);
		double y    = h3->GetYaxis()->GetBinCenter(biny);
		
		double q = pow(10,y);
		double npe = pow(q/0.0025,2); // q for where 1 pe is expected
		double w = pow(1.-exp(-npe),nlay);
		
		if (binx==10 && y<-2.2 && y>-2.4){
			cout<<"q "<<q<<" npe "<<npe<<" w "<<w<<endl;
		}
		
		h3->Fill(x,y,w);
          
       }
    }
    
    h3->SetStats(0);
    h3->Draw("colz");
    h3->SaveAs("eff.C");
}
