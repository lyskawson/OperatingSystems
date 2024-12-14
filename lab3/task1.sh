#!/bin/bash

#ps
# -f: Wyświetla rozszerzone informacje o procesach 
# -l: Pokazuje szczegółowe dane techniczne o procesach 
# -u: Filtruje procesy według użytkownika, wyświetlając tylko te, które należą do określonego użytkownika
# -t: Wyświetla procesy powiązane z określonym terminalem 
# -p: Pokazuje informacje o procesach na podstawie ich identyfikatorów 

#zadanie 1 
ps -fu $(whoami) | wc -l
#liczba procesów: 

#zadanie 2 
pgrep sshd | wc -l
#liczba procesów: 
ps -f -p $(pgrep sshd)
