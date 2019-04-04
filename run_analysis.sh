#!/bin/bash
mkdir -p ./dataqcd
mkdir -p ./datagammaZ
mkdir -p ./dataonia
mkdir -p ./pythialogfiles

killall py_sim
sleep 1

make

NUM_START=1
NUM_END=1 #5
EVENTS_PER_FILE=100000

for type in "0" "1" "2"; do
for mass in "0.01" "0.02" "0.04" "0.054" "0.081" "0.15" "0.219" "0.329" "0.5" "0.75" "1" "1.4" "1.6" "2" "4" "5" "6" "10" "20" "30" "40" "42" "44" "46" "48" "50" "60" "80" "100" "150" "200" "300"
do
  for ((i=NUM_START;i<=NUM_END;i++)); do
    if [ ${type} -le 1 ]; then 
      if [ ${mass%.*} -ge 10 ]; then continue; fi
    fi
    typename="qcd"
    if [ ${type} -eq 1 ]; then typename="onia"; fi
    if [ ${type} -eq 2 ]; then typename="gammaZ"; fi
    echo $type $typename $mass $i
    ./py_sim -t ${type} -m ${mass} -n ${EVENTS_PER_FILE} -f data${typename}/${mass}_${i}.root > pythialogfiles/${typename}_${mass}_${i}.txt &
    while [ `pgrep -c py_sim` -ge 8 ]; do sleep 10; done # wait for jobs to finish before starting new ones
  done
done
done

echo "waiting for last jobs to finish"
while [ `pgrep -c py_sim` -ge 1 ]; do sleep 10; done # wait for jobs to finish
ls dataqcd/ > filenamesqcd.txt
ls datagammaZ/ > filenamesgammaZ.txt
ls dataonia/ > filenamesonia.txt
echo "done!"
