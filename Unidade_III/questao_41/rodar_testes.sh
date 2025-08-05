#!/bin/bash

for p in 4
do
    echo "Rodando com $p processos"
    for i in {1..3}
    do
        mpiexec -n $p mpi_41_a 200000000
    done
done
