#!/bin/bash

for p in 1 2 4
do
    echo "Rodando com $p processos"
    for i in {1..5}
    do
        mpiexec -n $p mpi_odd_even g 33554432
    done
done
