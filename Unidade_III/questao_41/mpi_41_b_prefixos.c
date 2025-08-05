/* File:     mpi_41_a.c
 *
 * Compile:  mpicc -g -Wall -o mpi_41_b_prefixos mpi_41_b_prefixos.c -lm
 * Usage:    mpiexec -n<number of processes> ./mpi_41_b_prefixos
 */
#include <stdio.h>
#include <mpi.h> 
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*---------------------------------------------------------------------*/
void Prefix_sum_ring(int *prefix, int my_rank, int size, int *my_val, int n, MPI_Comm comm);

int main(int argc, char* argv[]) {
    int comm_sz, my_rank, n;
    int *my_vals = NULL;
    int *prefix = NULL;
    double time_start, time_finish, timer, time_max;

    MPI_Init(NULL, NULL); 
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
    prefix = (int*)malloc(n * sizeof(int));

    srand(time(NULL) + my_rank);
    for (int i = 0; i < n; i++) {
        my_vals[i] = rand() % 10;
    }

    printf("Processo %d: meus valores: ", my_rank);
    for (int i = 0; i < n; i++) {
        printf("%d ", my_vals[i]);
    }
    printf("\n");

    MPI_Barrier(MPI_COMM_WORLD);
    time_start = MPI_Wtime();
    Prefix_sum_ring(prefix, my_rank, comm_sz, my_vals, n, MPI_COMM_WORLD);
    time_finish = MPI_Wtime();

    timer = time_finish - time_start;
    MPI_Reduce(&timer, &time_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  
    if (my_rank == 0) {
        printf("Tempo máximo: %f segundos\n", time_max);
    }

    printf("Processo %d: prefixo: ", my_rank);
    for (int i = 0; i < n; i++) {
        printf("%d ", prefix[i]);
    }
    printf("\n");

    free(my_vals);
    free(prefix);
    MPI_Finalize();
    return 0;
}

/*---------------------------------------------------------------------*/
void Prefix_sum_ring(int *prefix, int my_rank, int size, int *my_val, int n, MPI_Comm comm) {
    int *recv_buf = (int*)malloc(n * sizeof(int));
    int source = (my_rank - 1 + size) % size;
    int dest = (my_rank + 1) % size;

    for (int i = 0; i < n; i++)
        prefix[i] = my_val[i];

    int *temp_sum = (int*)malloc(n * sizeof(int));
    if (my_rank == 0) {
        for (int i = 0; i < n; i++)
            temp_sum[i] = my_val[i];
        MPI_Send(temp_sum, n, MPI_INT, dest, 0, comm);
    } else {
        MPI_Recv(recv_buf, n, MPI_INT, source, 0, comm, MPI_STATUS_IGNORE);
        for (int i = 0; i < n; i++)
            prefix[i] = recv_buf[i] + my_val[i];

        if (my_rank != size - 1) {
            for (int i = 0; i < n; i++)
                temp_sum[i] = prefix[i];
            MPI_Send(temp_sum, n, MPI_INT, dest, 0, comm);
        }
    }

    free(recv_buf);
    free(temp_sum);
}