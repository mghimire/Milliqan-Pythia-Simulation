#wget https://www.dropbox.com/s/q76sfinwimys77j/data14pt2.zip
#unzip data14pt2.zip
#rm data14pt2.zip

# ./plotgraphs.sh dataextra type rock dohist
#$1 extra mode is for extensions (suffix) to data directories
#$2 type mode is for pT cut type (0 for none, 1 for flat, and 2 for triangular)
#$3 rock mode is for rock damping cut (0 for off, 1 for on)
#$4 eff mode is for the SLP efficiency we are using (0 for 100% eff and 1-6 for the 6 SLPs of demonstrator)
#$5 minNPE mode is to set the minimum number of NPE required to set efficiency
#hist mode is for debugging hists (0 for off, 1 for on)
#dirname mode is to set the name of directory to save plots into

dohist=0 # make debugging histograms
dirname=newplots


#do this plot generation once per dataset and comment out for limitplot analyses
#root -b -l -q plotgraph.cc\(\"gammaZ$1\",$2,$3,$dohist,\"$dirname\"\)
#sleep 1
#root -b -l -q plotgraph.cc\(\"onia$1\",$2,$3,$dohist,\"$dirname\"\)
#sleep 1
#root -b -l -q plotgraph.cc\(\"qcd$1\",$2,$3,$dohist,\"$dirname\"\)
#sleep 1

#nlay=4 # 1 (full eff), 3, or 4 #options for old 
#root -l -q filleff.cc\($nlay,$dirname\)

root -b -l -q SLPeff.cc\(\"$4\",$5,\"$dirname\"\)

changedname=${5/./p}
echo changedname

doqcd=1 # 0 or 1
bkg=0 # 0 (demonstrator) or 1 (orig)

root -b -l -q $dirname/heatplotpTweight_type$2\_rock$3\_qcd$1.C $dirname/heatplotpTweighterr_type$2\_rock$3\_qcd$1.C $dirname/heatplotpTweight_type$2\_rock$3\_onia$1.C $dirname/heatplotpTweighterr_type$2\_rock$3\_onia$1.C $dirname/heatplotpTweight_type$2\_rock$3\_gammaZ$1.C $dirname/heatplotpTweighterr_type$2\_rock$3\_gammaZ$1.C $dirname/pathTuple$4/minNPE$changedname/SLP$4\_minNPE$changedname.C backgroundPerHour.C limitplot.cc\($doqcd,$bkg,\"$1\"\,$2,$3,$5,\"$dirname\"\)
