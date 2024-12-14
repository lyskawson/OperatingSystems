#!/bin/bash

file="$1"

if [ -z "$file" ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

pipe_name="/tmp/mypipe"
mkfifo "$pipe_name"

# Funkcja, która będzie wykonywana przez proces potomny
child_process() {
    while true; do
        # Odczytaj dane z potoku
        if read line < "$pipe_name"; then
            # Dodaj znaczniki i wyświetl linię
            echo "@@@@ $line ####"
        else
            break
        fi
    done
}

# Uruchom proces potomny
child_process &

# Otwórz plik i wysyłaj zawartość do potoku
while IFS= read -r line; do
    echo "$line" > "$pipe_name"
done < "$plik"

# Zakończ proces potomny
kill $!
rm "$pipe_name"
