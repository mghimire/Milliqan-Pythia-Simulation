#!/bin/bash

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
	./plotgraphs.sh 14pt2 2 1 ${SLPname} ${minNPE}
	done
done

