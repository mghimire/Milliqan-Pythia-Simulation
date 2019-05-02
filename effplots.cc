void effplots(TString extra=""){
  TH2D* mCPseennocut = (TH2D*)gROOT->Get("mCPseen"+extra+"type0_rock0");
  if (!mCPseennocut) cout<<"couldn't find mCPseennocut"<<endl;
  TH2D* mCPseenflat = (TH2D*)gROOT->Get("mCPseen"+extra+"type1_rock0");
  if (!mCPseenflat) cout<<"couldn't find mCPseenflat"<<endl;
  TH2D* mCPseentri = (TH2D*)gROOT->Get("mCPseen"+extra+"type2_rock0");
  if (!mCPseentri) cout<<"couldn't find mCPseentri"<<endl;
  TH2D* mCPseenflatrock = (TH2D*)gROOT->Get("mCPseen"+extra+"type1_rock1");
  if (!mCPseenflatrock) cout<<"couldn't find mCPseenflatrock"<<endl;
  TH2D* mCPseentrirock = (TH2D*)gROOT->Get("mCPseen"+extra+"type2_rock1");
  if (!mCPseentrirock) cout<<"couldn't find mCPseentrirock"<<endl;

  TCanvas *c3 = new TCanvas("c3","mCP_canvas",0,0,600,400);
  c3->SetLogz(1);
  mCPseenflat->Draw();
  mCPseenflatrock->Draw();
  mCPseenflatrock->Divide(mCPseenflat);	
  mCPseenflatrock->SetTitle("Efficiency of Rock Effect after Flat pT cut");
  //mCPseenflatrock->SetMinimum(.5);
  mCPseenflatrock->Draw("colz");
  mCPseenflatrock->SetContour(0);
  
  gSystem->Exec("mkdir plots");	
  c3->SaveAs("plots/flatrockeff_"+extra+".C");
  c3->SaveAs("plots/flatrockeff_"+extra+".pdf");

  mCPseentri->Draw();
  mCPseentrirock->Draw();
  mCPseentrirock->Divide(mCPseentri);
  mCPseentrirock->SetTitle("Efficiency of Rock Effect after Triangular pT cut");
  //mCPseentrirock->SetMinimum(.5);
  mCPseentrirock->Draw("colz");
  mCPseentrirock->SetContour(0);
  
  c3->SaveAs("plots/trirockeff_"+extra+".C");
  c3->SaveAs("plots/trirockeff_"+extra+".pdf");

  mCPseennocut->Draw();
  mCPseentri->Draw();
  mCPseentri->Divide(mCPseennocut);	
  mCPseentri->SetTitle("Efficiency of Triangular pT cut");
  mCPseentri->SetMinimum(1e-8);
  mCPseentri->Draw("colz");
  mCPseentri->SetContour(0);
 
  c3->SaveAs("plots/trieff_"+extra+".C");
  c3->SaveAs("plots/trieff_"+extra+".pdf");

  mCPseennocut->Draw();
  mCPseenflat->Draw();
  mCPseenflat->Divide(mCPseennocut);	
  mCPseenflat->SetTitle("Efficiency of Flat pT cut");
  mCPseenflat->SetMinimum(1e-8);
  mCPseenflat->Draw("colz");
  mCPseenflat->SetContour(0);
  
  c3->SaveAs("plots/flateff_"+extra+".C");
  c3->SaveAs("plots/flateff_"+extra+".pdf");
}
