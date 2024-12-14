#!/bin/bash

email_list="emails.txt"

while read -r email_address; do
    if [ -n "$email_address" ]; then #jesli nie jest puste 
        echo "Wiadomosc" | mail -s "Temat" "$email_address" 
        echo "E-mail sent to: $email_address" 
    fi
done < "$email_list"
