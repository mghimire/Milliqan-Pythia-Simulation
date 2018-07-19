#!/bin/bash
source ~/.profile
mkdir -p ./data
declare -a masses=("2"
                   "0.5"
                   "0.01"
                   )
NUM_FILES=3
EVENTS_PER_FILE=1000
for mass in "${masses[@]}"
do
  for ((i=1;i<=NUM_FILES;i++)); do
    screen -S ${mass//.}_${i} -d -m sh -c "source ~/.profile;./py_sim -p 0 -m ${mass} -n ${EVENTS_PER_FILE} -f data/${mass}_${i}.root"
    # tmux new-session -d -s ${mass//.}_${i} "./py_sim -p 0 -m ${mass} -n ${EVENTS_PER_FILE} -f data/${mass}_${i}.root"
  done 
done
