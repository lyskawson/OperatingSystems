#!/bin/sh

handle_sigwinch() 
{
  echo "WINDOWS CHANGE - redraw"
}
first_shutdown=false

handle_sigusr1() 
{
  if [ "$first_shutdown" = false ]; then
    echo "Are you sure you want to turn off the projector?"
    first_shutdown=true
    (sleep 5 && first_shutdown=false) & #background process
  else
    echo "Turning off the projector..."
    exit 0
  fi
}

trap '' SIGFPE
trap 'echo "WINDOWS CHANGE - redraw"' SIGWINCH
trap 'handle_sigusr1' SIGUSR1

echo "Projector is on... [PID: $$]"
while true; do
  sleep 1
done
