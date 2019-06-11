void backgroundPerHour()
{
//=========Macro generated from canvas: Canvas_1/Canvas_1
//=========  (Tue Jun  4 17:29:33 2019) by ROOT version 6.12/06
   TCanvas *Canvas_1 = new TCanvas("Canvas_1", "Canvas_1",604,94,1002,534);
   Canvas_1->Range(-4.213659,-3.635353,4.912629,0.3690577);
   Canvas_1->SetFillColor(0);
   Canvas_1->SetBorderMode(0);
   Canvas_1->SetBorderSize(2);
   Canvas_1->SetLogx();
   Canvas_1->SetLogy();
   Canvas_1->SetFrameBorderMode(0);
   Canvas_1->SetFrameBorderMode(0);
   Double_t xAxis1[10] = {0, 0.5, 1.5, 5, 10, 20, 50, 100, 1000, 10000}; 
   
   TH1D *A__1 = new TH1D("A__1","deltaTWithintimingSelStraightPred",9, xAxis1);
   A__1->SetBinContent(1,0.01193878);
   A__1->SetBinContent(2,0.4702041);
   A__1->SetBinContent(3,0.05326531);
   A__1->SetBinContent(4,0.01469388);
   A__1->SetBinContent(5,0.004591837);
   A__1->SetBinContent(6,0.002755102);
   A__1->SetBinError(1,0.003311221);
   A__1->SetBinError(2,0.02078028);
   A__1->SetBinError(3,0.006994077);
   A__1->SetBinError(4,0.003673469);
   A__1->SetBinError(5,0.002053532);
   A__1->SetBinError(6,0.001590659);
   A__1->SetEntries(607);
   A__1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   A__1->SetLineColor(ci);
   A__1->GetXaxis()->SetTitle("nPE");
   A__1->GetXaxis()->SetLabelFont(42);
   A__1->GetXaxis()->SetLabelSize(0.035);
   A__1->GetXaxis()->SetTitleSize(0.035);
   A__1->GetXaxis()->SetTitleFont(42);
   A__1->GetYaxis()->SetLabelFont(42);
   A__1->GetYaxis()->SetLabelSize(0.035);
   A__1->GetYaxis()->SetTitleSize(0.035);
   A__1->GetYaxis()->SetTitleOffset(0);
   A__1->GetYaxis()->SetTitleFont(42);
   A__1->GetZaxis()->SetLabelFont(42);
   A__1->GetZaxis()->SetLabelSize(0.035);
   A__1->GetZaxis()->SetTitleSize(0.035);
   A__1->GetZaxis()->SetTitleFont(42);
   A__1->Draw("text");
   
   TPaveText *pt = new TPaveText(0.2675,0.9363534,0.7325,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *pt_LaTex = pt->AddText("deltaTWithintimingSelStraightPred");
   pt->Draw();
   Canvas_1->Modified();
   Canvas_1->cd();
   Canvas_1->SetSelected(Canvas_1);
}
