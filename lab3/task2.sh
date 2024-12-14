bash -c 'echo "Start Process A"; sleep 1; echo "Start Process B"; sleep 1; echo "Start Process C"; sleep 120'

ps -ef --forest # --forest: przedstawia procesy w hirarchii , -e wszysktie procesy

kill <PID_B>

ps -ef --forest

#Proces "C" powinien mieć PPID równy 1, co oznacza, że został zaadoptowany przez proces init.
