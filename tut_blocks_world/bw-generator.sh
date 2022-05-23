#!/bin/bash

if [ $# -le 0 ]; then
    echo usage: $0 '<nblocks> <seed>'
    echo generates a Blocks World problem of '<nblocks>' blocks
    echo using the seed '<seed>' for generating the random problem.
    echo The problem is saved in 'bw-<nblocks>-<seed>.prob'
    echo
    echo If '<seed>' is not passed as parameters,
    echo a problem 'bw-<nblocks>.prob' is created using some seed.
    exit
fi

if [ $# -le 1 ]; then
    ./bwstates/bwstates -n $1 -s 2 > bw-$1.prob
else
    ./bwstates/bwstates -n $1 -r $2 -s 2 > bw-$1-$2.prob
fi



