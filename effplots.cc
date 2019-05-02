void effplots(TString extra=""){
  TH2D* mCPseennocut = (TH2D*)gROOT->Get("mCPseen"+extra+"type0_rock0");
  TH2D* mCPseenflat = (TH2D*)gROOT->Get("mCPseen"+extra+"type1_rock0");
  TH2D* mCPseentri = (TH2D*)gROOT->Get("mCPseen"+extra+"type2_rock0");
  TH2D* mCPseenflatrock = (TH2D*)gROOT->Get("mCPseen"+extra+"type1_rock1");
  TH2D* mCPseentrirock = (TH2D*)gROOT->Get("mCPseen"+extra+"type2_rock1");

  TCanvas *c3 = new TCanvas("c3","mCP_canvas",0,0,600,400);
  c3->SetLogz(1);
  mCPseenflat->Draw();
  mCPseenflatrock->Draw();
  mCPseenflatrock->Divide(mCPseenflat);	
  mCPseenflatrock->SetTitle("Efficiency of Rock Effect after Flat pT cut");
  mCPseenflatrock->Draw("colz");
  
  gSystem->Exec("mkdir plots");	
  c3->SaveAs("plots/flatrockeff_"+extra+".C");
  c3->SaveAs("plots/flatrockeff_"+extra+".pdf");

  mCPseentri->Draw();
  mCPseentrirock->Draw();
  mCPseentrirock->Divide(mCPseentri);
  mCPseentrirock->SetTitle("Efficiency of Rock Effect after Triangular pT cut");
  mCPseentrirock->Draw("colz");
  
  c3->SaveAs("plots/trirockeff_"+extra+".C");
  c3->SaveAs("plots/trirockeff_"+extra+".pdf");

  mCPseenflat->Draw();
  mCPseentri->Draw();
  mCPseentri->Divide(mCPseenflat);	
  mCPseentri->SetTitle("Efficiency of Triangular pT cut after Flat pT cut");
  mCPseentri->Draw("colz");
  
  c3->SaveAs("plots/flattrieff_"+extra+".C");
  c3->SaveAs("plots/flattrieff_"+extra+".pdf");

  mCPseennocut->Draw();
  mCPseenflat->Draw();
  mCPseenflat->Divide(mCPseennocut);	
  mCPseenflat->SetTitle("Efficiency of Flat pT cut");
  mCPseenflat->Draw("colz");
  
  c3->SaveAs("plots/flateff_"+extra+".C");
  c3->SaveAs("plots/flateff_"+extra+".pdf");
}
