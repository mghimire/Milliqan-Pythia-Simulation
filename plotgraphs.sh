
root -l -q 'plotgraph.cc("gammaZ")'
root -l -q 'plotgraph.cc("onia")'
root -l -q 'plotgraph.cc("qcd")'

#root heatplotpTweight_qcd.C heatplotpTweight_onia.C heatplotpTweight_gammaZ.C 'limitplot.C(0)' #don't add qcd
root heatplotpTweight_qcd.C heatplotpTweight_onia.C heatplotpTweight_gammaZ.C 'limitplot.C(1)' #all

