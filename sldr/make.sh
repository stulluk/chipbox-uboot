#!/bin/bash

rm -f ./sldr/*.bin
gcc -I ./include -o ./sldr/ddr_bin_gen ./sldr/ddr_bin_gen.c
./sldr/ddr_bin_gen
