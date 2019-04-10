void oldlimits(){

Int_t n=11;
Float_t x[] = {45,2,.6, .4,.1, .1,.01, .01, 200, 200, 45};
Float_t y[] = {.25,.25,.1, .035,.03, .0001,.0001, 1, 1, .3, .3};
Float_t lx[n];
Float_t ly[n];
for (int i=0; i<n; ++i){
	lx[i]=log10(x[i]);
	ly[i]=log10(y[i]);
}

TPolyLine *polyLine = new TPolyLine(n, lx, ly, "F");

polyLine->SetFillColor(38);
polyLine->SetFillStyle(3003);

polyLine->Draw("f");

}
