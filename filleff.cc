void filleff(int nlay=1, TString dirname="plots"){

	TH2D* heff = new TH2D("heff","heff", 41, -2, 2.3, 42, -4, 0.01);
	
	if (nlay==1) heff->SetTitle("unity");
	if (nlay==3) heff->SetTitle("3lay");
	if (nlay==4) heff->SetTitle("4lay");
	
	Int_t nbinsx = heff->GetNbinsX();
    Int_t nbinsy = heff->GetNbinsY();
    
    for (int biny=1;biny<=nbinsy;biny++) {
       for (int binx=1;binx<=nbinsx;binx++) {
          
        	double x    = heff->GetXaxis()->GetBinCenter(binx);
		double y    = heff->GetYaxis()->GetBinCenter(biny);
		
		double q = pow(10,y);
		double qfor1pe = 0.0025;
		if (nlay==4) qfor1pe*=4./3.; // account for shorter 4-layer bars
		double npe = pow(q/qfor1pe,2); // q for where 1 pe is expected
		double w = pow(1.-exp(-npe),nlay);
		
		if (binx==10 && y<-2.2 && y>-2.4){
			cout<<"q "<<q<<" npe "<<npe<<" w "<<w<<endl;
		}
		
		if (nlay==1) w=1.0;
		heff->Fill(x,y,w);
          
       }
    }
    
    TCanvas *c1 = new TCanvas("c1","mCP_canvas",0,0,600,400);
    heff->SetStats(0);
    heff->Draw("colz");
    gSystem->Exec("mkdir -p "+dirname);
    if (nlay==1) {
		c1->SaveAs(dirname+"/eff1.C");//unity efficiency
		c1->SaveAs(dirname+"/eff1.pdf");
	}
    else if (nlay==3) {
		c1->SaveAs(dirname+"/eff3.C");//3 layer eff
		c1->SaveAs(dirname+"/eff3.pdf");
	}
    else if (nlay==4) {
		c1->SaveAs(dirname+"/eff4.C");//4 layer eff
		c1->SaveAs(dirname+"/eff4.pdf");
	}
    else cout<<"dont know layer "<<nlay<<endl;
	
}
