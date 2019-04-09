
#root -l -q plotgraph.cc\(\"gammaZ$1\"\)
#sleep 1
#root -l -q plotgraph.cc\(\"onia$1\"\)
#sleep 1
#root -l -q plotgraph.cc\(\"qcd$1\"\)
#sleep 1

root -l -q filleff.cc\(1\)
root -l -q filleff.cc\(3\)
root -l -q filleff.cc\(4\)

doqcd=0 # 0 or 1
bkg=0 # 0 (optimistic) or 1 (orig)

root -l -q plots/heatplotpTweight_qcd1.C plots/heatplotpTweight_onia1.C plots/heatplotpTweight_gammaZ1.C plots/eff1.C limitplot.cc\($doqcd,$bkg,\"$1\"\)
root -l -q plots/heatplotpTweight_qcd1.C plots/heatplotpTweight_onia1.C plots/heatplotpTweight_gammaZ1.C plots/eff3.C limitplot.cc\($doqcd,$bkg,\"$1\"\)
root -l -q plots/heatplotpTweight_qcd1.C plots/heatplotpTweight_onia1.C plots/heatplotpTweight_gammaZ1.C plots/eff4.C limitplot.cc\($doqcd,$bkg,\"$1\"\)
