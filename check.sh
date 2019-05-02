#!/bin/bash
dohist=0 # make debugging histograms
echo root -l -q \'plotgraph.cc\(\"gammaZ$1\", $2, $3, $dohist\)\'
