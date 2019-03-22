#!/bin/bash
mkdir -p ./datapT4
mkdir -p ./pythialogfiles
declare -a masses=( "0.01"
                    "0.02"
                    "0.04"
                    "0.054"
                    "0.081"
                    "0.15"
                    "0.219"
                    "0.329"
                    "0.5"
                    "0.75"
                    "1"
                    "2"
                    "4"
                    "5"
                    "10"
                   )

NUM_START=1
NUM_END=1
EVENTS_PER_FILE=5000000
for mass in "${masses[@]}"
do
  for ((i=NUM_START;i<=NUM_END;i++)); do
    #screen -S ${mass//.}_${i} -d -m sh -c "source ~/.profile;./py_sim -p 0 -m ${mass} -n ${EVENTS_PER_FILE} -f data/${mass}_${i}.root"
    ./py_sim -m ${mass} -n ${EVENTS_PER_FILE} -f datapT4/${mass}_${i}.root > pythialogfiles/${mass}_${i}.txt
  done
done

ls data/ > filenamespT4.txt
