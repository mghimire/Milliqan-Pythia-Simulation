#!/bin/bash
echo "saving to dirs ening in $1 "
mkdir -p ./dataqcd$1
mkdir -p ./datagammaZ$1
mkdir -p ./dataonia$1
mkdir -p ./pythialogfiles$1

killall py_sim
sleep 1

make

NUM_START=1
NUM_END=1 #5
EVENTS_PER_FILE=100000

nthreads=8

nmass=40
for type in "0" "1" "2"; do  
for (( massi=0; massi<=$nmass; massi++ )); do
echo $nmass $massi
  mass=`python -c "print 0.01*pow(10,(4.3 * ${massi} / ${nmass} ))"`
  for ((i=NUM_START;i<=NUM_END;i++)); do
    if [ ${type} -le 1 ]; then 
      if [ ${mass%.*} -ge 8 ]; then continue; fi
    fi
    typename="qcd"
    if [ ${type} -eq 1 ]; then typename="onia"; fi
    if [ ${type} -eq 2 ]; then typename="gammaZ"; fi
    echo $type $typename $mass $i
    ./py_sim -t ${type} -m ${mass} -n ${EVENTS_PER_FILE} -f data${typename}${1}/${mass}_${i}.root > pythialogfiles${1}/${typename}_${mass}_${i}.txt &
    while [ `pgrep -c py_sim` -ge $nthreads ]; do sleep 10; done # wait for jobs to finish before starting new ones
  done
done
done

echo "waiting for last jobs to finish"
while [ `pgrep -c py_sim` -ge 1 ]; do sleep 10; done # wait for jobs to finish
ls dataqcd${1}/ > filenamesqcd${1}.txt
ls datagammaZ${1}/ > filenamesgammaZ${1}.txt
ls dataonia${1}/ > filenamesonia${1}.txt
echo "done!"
