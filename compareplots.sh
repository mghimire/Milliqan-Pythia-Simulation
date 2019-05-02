#compare and divide total mCP seen heatplots for various cuts
#name are like "effheatplotpTweight_type0_rock0_14pt2_qcd_optbkg_unity"

#arguments: 
#1: extra string (from run_analysis)
#2: qcd or noqcd
#3: bkgtype
#4: nlay

#so run like: 
#./compareplots.sh 14pt2 qcd optbkg unity

./plotgraphs.sh $1 0 0
./plotgraphs.sh $1 1 0
./plotgraphs.sh $1 1 1
./plotgraphs.sh $1 2 0
./plotgraphs.sh $1 2 1

root -l -q plots/effheatplotpTweight_type0_rock0_$1\_$2\_$3\_$4.C plots/effheatplotpTweight_type1_rock0_$1\_$2\_$3\_$4.C plots/effheatplotpTweight_type1_rock1_$1\_$2\_$3\_$4.C plots/effheatplotpTweight_type2_rock0_$1\_$2\_$3\_$4.C plots/effheatplotpTweight_type2_rock1_$1\_$2\_$3\_$4.C effplots.cc\(\"$1\"\)
