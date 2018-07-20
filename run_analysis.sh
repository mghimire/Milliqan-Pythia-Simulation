#!/bin/bash
source ~/.profile
mkdir -p ./data
declare -a masses=("10"
                   "2"
                   "1"
                   )
NUM_START=1
NUM_END=2
EVENTS_PER_FILE=5000000
for mass in "${masses[@]}"
do
  for ((i=NUM_START;i<=NUM_END;i++)); do
    # screen -S ${mass//.}_${i} -d -m sh -c "source ~/.profile;./py_sim -p 0 -m ${mass} -n ${EVENTS_PER_FILE} -f data/${mass}_${i}.root"
    tmux new-session -d -s ${mass//.}_${i} "./py_sim -p 0 -m ${mass} -n ${EVENTS_PER_FILE} -f data/${mass}_${i}.root"
  done
done
