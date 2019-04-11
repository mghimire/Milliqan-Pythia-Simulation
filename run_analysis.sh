#!/bin/bash

echo "saving to dirs ening in $1 "
mkdir -p ./dataqcd$1
mkdir -p ./datagammaZ$1
mkdir -p ./dataonia$1
mkdir -p ./pythialogfiles$1

killall py_sim
sleep 1

source environ.sh
make all

if [ -z "$2" ]; then NUM_START=1; else NUM_START=$2; fi
NUM_END=$(($NUM_START + 4))
DEF_EVENTS_PER_FILE=20000
DO_EXTRA_EVENTS=1 # set to 1 to do extra events for masses that need it, or 0 for a quicker life
NTHREADS=32

nmass=40
for type in "0" "1" "2"; do
for (( massi=0; massi<=$nmass; massi++ )); do
  mass=`python -c "print 0.01*pow(10,(4.3 * ${massi} / ${nmass} ))"`
  for ((i=NUM_START;i<=NUM_END;i++)); do
    EVENTS_PER_FILE=$DEF_EVENTS_PER_FILE
    if [ ${type} -eq 0 ]; then 
      if [ ${mass%.*} -ge 8 ]; then continue; fi
      if [ $DO_EXTRA_EVENTS -eq 1 ]; then
		if (( $(echo "${mass} > 0.3" |bc -l) )); then EVENTS_PER_FILE=`python -c "print 10*${DEF_EVENTS_PER_FILE}"`; fi
		if (( $(echo "${mass} > 1.0" |bc -l) )); then EVENTS_PER_FILE=`python -c "print 100*${DEF_EVENTS_PER_FILE}"`; fi
      fi
      typename="qcd";
    fi
    if [ ${type} -eq 1 ]; then
      if [ ${mass%.*} -ge 8 ]; then continue; fi
      if [ $DO_EXTRA_EVENTS -eq 1 ]; then
		if (( $(echo "${mass} > 1.4" |bc -l) )); then EVENTS_PER_FILE=`python -c "print 10*${DEF_EVENTS_PER_FILE}"`; fi
		if (( $(echo "${mass} > 3.8" |bc -l) )); then EVENTS_PER_FILE=`python -c "print 100*${DEF_EVENTS_PER_FILE}"`; fi
	  fi
      typename="onia";
    fi
    if [ ${type} -eq 2 ]; then
      if [ $DO_EXTRA_EVENTS -eq 1 ]; then
		if (( $(echo "${mass} > 6.0" |bc -l) )); then EVENTS_PER_FILE=`python -c "print 10*${DEF_EVENTS_PER_FILE}"`; fi
		if (( $(echo "${mass} == 35.2776786619" |bc -l) )); then EVENTS_PER_FILE=`python -c "print 100*${DEF_EVENTS_PER_FILE}"`; fi
	  fi
      typename="gammaZ";
    fi
    echo $type $typename $mass $i $EVENTS_PER_FILE
    ./py_sim -t ${type} -m ${mass} -n ${EVENTS_PER_FILE} -f data${typename}${1}/${mass}_${i}.root >& pythialogfiles${1}/${typename}_${mass}_${i}.txt &
    while [ `pgrep -c py_sim` -ge $NTHREADS ]; do sleep 10; done # wait for jobs to finish before starting new ones
  done
done
done

echo "waiting for last jobs to finish"
while [ `pgrep -c py_sim` -ge 1 ]; do sleep 10; done # wait for jobs to finish
ls dataqcd${1}/ > filenamesqcd${1}.txt
ls datagammaZ${1}/ > filenamesgammaZ${1}.txt
ls dataonia${1}/ > filenamesonia${1}.txt
echo "done!"

#zip it
#zip data$1.zip -r *$1 *$1.txt

#make all the plots
#./plotgraphs.sh $1

