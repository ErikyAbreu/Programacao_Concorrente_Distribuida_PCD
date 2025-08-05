/* File:     mpi_41_a.c
 *
 * Compile:  mpicc -g -Wall -o mpi_41_a_borboleta mpi_41_a_borboleta.c -lm
 * Usage:    mpiexec -n<number of processes> ./mpi_41_a_borboleta
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

void Allreduce_butterfly(int *total, int my_rank, int size, int *my_vals, int n, MPI_Comm comm);

int main(int argc, char* argv[]) {
    int comm_sz, my_rank;
    int *my_vals = NULL;
    int *total = NULL;
    int n;
    float time_start, time_finish, timer, time_a;

    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

    if (argc != 2) {
        if (my_rank == 0)
            fprintf(stderr, "Uso: %s <tamanho_do_vetor>\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }

    n = atoi(argv[1]);
    my_vals = (int*)malloc(n * sizeof(int));
    total = (int*)malloc(n * sizeof(int));

    srand(time(NULL) + my_rank);
    for (int i = 0; i < n; i++) {
        my_vals[i] = rand() % 100;
    }

    //printf("Processo %d, meus valores: ", my_rank);
    for (int i = 0; i < n; i++) {
        //printf("%d ", my_vals[i]);
    }
    //printf("\n");
    fflush(stdout);

    MPI_Barrier(MPI_COMM_WORLD);

    if (my_rank == 0) {
        //printf("----Allreduce_butterfly----\n", n);
        fflush(stdout);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    time_start = MPI_Wtime();
    Allreduce_butterfly(total, my_rank, comm_sz, my_vals, n, MPI_COMM_WORLD);
    time_finish = MPI_Wtime();

    timer = time_finish - time_start;
    MPI_Reduce(&timer, &time_a, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("TEMPO: %f segundos\n", time_a);
    }

    //printf("Processo %d, soma total: ", my_rank);
    for (int i = 0; i < n; i++) {
        //printf("%d ", total[i]);
    }
    //printf("\n");
    fflush(stdout);

    free(my_vals);
    free(total);
    MPI_Finalize();
    return 0;
}

void Allreduce_butterfly(int *total, int my_rank, int size, int *my_vals, int n, MPI_Comm comm) {
    int etapas = (int)log2(size);
    int parceiro;
    int* temp_vals = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        total[i] = my_vals[i];
    }

    for (int i = 0; i < etapas; i++) {
        parceiro = my_rank ^ (1 << i);

        for (int j = 0; j < n; j++) {
            temp_vals[j] = total[j];
        }

        MPI_Sendrecv_replace(temp_vals, n, MPI_INT, parceiro, 0, parceiro, 0, comm, MPI_STATUS_IGNORE);

        for (int j = 0; j < n; j++) {
            total[j] += temp_vals[j];
        }
    }

    free(temp_vals);
}