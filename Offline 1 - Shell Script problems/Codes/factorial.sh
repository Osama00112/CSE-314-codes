#!/bin/bash

# fact(i) = i * fact(i-1)

fact() {
    if [[ $1 -eq 0 ]]; then
        echo 1
        return
    fi

    last=$(fact $(( $1 - 1 )))

    echo $(( $1 * last ))
}

fact $1