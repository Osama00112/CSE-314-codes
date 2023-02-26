#!/bin/bash

rep(){
    for file in $*; do
        if [[ -d "$file" ]]; then
            cd "$file"
                rep $(ls)
            cd ..
        elif [[ "$file" == *.c ]]; then
            mv "$file" "${file%.c}.cpp"
        fi

    done

}

rep $(ls)