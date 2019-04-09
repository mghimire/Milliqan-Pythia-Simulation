
root -l -q plotgraph.cc\(\"gammaZ$1\"\)
sleep 1
root -l -q plotgraph.cc\(\"onia$1\"\)
sleep 1
root -l -q plotgraph.cc\(\"qcd$1\"\)
sleep 1

root -l -q filleff.C\(1\)
root -l -q filleff.C\(3\)
root -l -q filleff.C\(4\)

doqcd=0 # 0 or 1
bkg=0 # 0 (optimistic) or 1 (orig)

root -l -q heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C eff1.C limitplot.C\($doqcd,$bkg,\"$1\"\)
root -l -q heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C eff3.C limitplot.C\($doqcd,$bkg,\"$1\"\)
root -l -q heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C eff4.C limitplot.C\($doqcd,$bkg,\"$1\"\)

doqcd=1 # 0 or 1
bkg=0 # 0 (optimistic) or 1 (orig)

root -l -q heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C eff1.C limitplot.C\($doqcd,$bkg,\"$1\"\)
root -l -q heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C eff3.C limitplot.C\($doqcd,$bkg,\"$1\"\)
root -l -q heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C eff4.C limitplot.C\($doqcd,$bkg,\"$1\"\)

doqcd=0 # 0 or 1
bkg=1 # 0 (optimistic) or 1 (orig)

root -l -q heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C eff1.C limitplot.C\($doqcd,$bkg,\"$1\"\)
root -l -q heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C eff3.C limitplot.C\($doqcd,$bkg,\"$1\"\)
root -l -q heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C eff4.C limitplot.C\($doqcd,$bkg,\"$1\"\)

