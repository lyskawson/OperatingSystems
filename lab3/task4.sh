
#wykorzystane polecenie: while true; do echo "Processing..."; sleep 1; done | cat | cat | cat
# ps -ef --forest | grep 'cat'

#Procesy w potoku sa w relacji rodzic-potomek, gdzie pierwszy proces (np. while) 
#jest rodzicem dla kolejnych procesow (cat). 


