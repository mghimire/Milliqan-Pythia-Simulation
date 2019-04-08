
root -l -q plotgraph.cc\(\"gammaZ$1\"\)
sleep 3
root -l -q plotgraph.cc\(\"onia$1\"\)
sleep 3
root -l -q plotgraph.cc\(\"qcd$1\"\)
sleep 3

#root heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C limitplot.C\(0,\"$1\"\) #don't add qcd
root heatplotpTweight_qcd$1.C heatplotpTweight_onia$1.C heatplotpTweight_gammaZ$1.C limitplot.C\(1,\"$1\"\) #all

