#!/bin/bash

if [ -z "$1" ]; then #sprawdza czy pierwszy argument jest pusty
  echo "Usage: $0 <PID>"
  exit 1
fi

projector_PID=$1

while true; do
  echo "Chose signal you want to send to projector:"
  echo "1 - SIGFPE"
  echo "2 - SIGWINCH"
  echo "3 - SIGUSR1"
  echo "4 - Exit"
  read -p "Choice: " choice

  case $choice in
    1) kill -SIGFPE $projector_PID ;;
    2) kill -SIGWINCH $projector_PID ;;
    3) kill -SIGUSR1 $projector_PID ;;
    4) exit 0 ;;
  esac
done

# c) kill -SIGKILL PID
#    sygnal nieprzechytywalny 
# d) kill -SIGSTOP PID, kill -SIGCONT PID
# e) kill -SIGSTOP PID
# f) bg wznowi proces w tle, a fg wznowi proces na pierwszym planie
#    SIGCONT ma ten sam efekt co bg