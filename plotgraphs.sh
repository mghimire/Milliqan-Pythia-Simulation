
root -l -q plotgraph.cc\(\"gammaZ$1\"\)
sleep 1
root -l -q plotgraph.cc\(\"onia$1\"\)
sleep 1
root -l -q plotgraph.cc\(\"qcd$1\"\)
sleep 1

nlay=4 # 1 (full eff), 3, or 4
root -l -q filleff.cc\($nlay\)

doqcd=1 # 0 or 1
bkg=0 # 0 (optimistic) or 1 (orig)

root -l -q plots/heatplotpTweight_qcd$1.C plots/heatplotpTweight_onia$1.C plots/heatplotpTweight_gammaZ$1.C plots/eff${nlay}.C limitplot.cc\($doqcd,$bkg,\"$1\"\)
