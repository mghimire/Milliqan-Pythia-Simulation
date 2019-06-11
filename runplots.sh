#!/bin/bash
#$1 extra mode is for extensions (suffix) to data directories
#$2 type mode is for pT cut type (0 for none, 1 for flat, and 2 for triangular)
#$3 rock mode is for rock damping cut (0 for off, 1 for on)

declare -a SLPnames=("4_8_12"
		     "5_9_13"
		     "16_22_24"
		     "0_2_6"
		     "1_3_7"
		     "17_23_25"
		     "All")

declare -a minNPEs=("0.0"
		    "0.5"
		    "1.5"
		    "5.0"
		    "10.0"
		    "20.0"
		    "50.0"
		    "100.0"
		    "1000.0")

for SLPname in "${SLPnames[@]}"
do
	for minNPE in "${minNPEs[@]}"
	do
	./plotgraphs.sh $1 $2 $3 ${SLPname} ${minNPE}
	done
done

