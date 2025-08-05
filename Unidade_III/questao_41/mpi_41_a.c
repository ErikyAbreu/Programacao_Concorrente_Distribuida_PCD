/* File:     mpi_41_a.c
 *
 * Compile:  mpicc -g -Wall -o mpi_41_a mpi_41_a.c -lm
 * Usage:    mpiexec -n<number of processes> ./mpi_41_a
 */
#include <stdio.h>
#include <mpi.h> 
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "timer.h"

void Allreduce_ring(int *total, int my_rank, int size, int *my_val, int n, MPI_Comm comm);

/*---------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
  int comm_sz, my_rank, n;
  int *my_vals = NULL;
  int *total = NULL;
  float time_start, time_finish, timer, time_a;

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
  total = (int*)malloc(n * sizeof(int));

  srand(time(NULL) + my_rank);
  for (int i = 0; i < n; i++) {
        my_vals[i] = rand() % 100;
  }
  printf("I am process %d and my val: ", my_rank);
  for (int i = 0; i < n; i++) {
        printf("%d ", my_vals[i]);
  }
  printf("\n");
  fflush(stdout);

  if(my_rank==0){
    printf("----Allreduce_ring----\n");
    fflush(stdout);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  time_start = MPI_Wtime();
  Allreduce_ring(total, my_rank, comm_sz, my_vals, n, MPI_COMM_WORLD);
  time_finish = MPI_Wtime();
  timer = time_finish - time_start;
  MPI_Reduce(&timer, &time_a, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  
  if(my_rank == 0){
    printf("TIME: %f\n", time_a);
  }

  printf("Processo %d, total: ", my_rank);
  for (int i = 0; i < n; i++) {
      printf("%d ", total[i]);
  }
  fflush(stdout);

  free(total);
  free(my_vals);
  MPI_Finalize();
  return 0;
}  /* main */

// Allreduce_ring -------------------------------------------------------------------

void Allreduce_ring(int *total, int my_rank, int size, int *my_vals, int n, MPI_Comm comm) {
  int i;
  int *temp_vals = (int*)malloc(n * sizeof(int));
  int dest, source;

  dest = (my_rank + 1) % size;
  source = (my_rank - 1 + size) % size;

  if (my_rank == (size - 1)) {
    dest = 0;
  } else if (my_rank == 0) {
    source = (size - 1) % size;
  }

  for (i = 0; i < n; i++) {
        total[i] = temp_vals[i] = my_vals[i];
  }

  for (int step = 1; step < size; step++) {
    MPI_Sendrecv_replace(temp_vals, n, MPI_INT, dest, step, source, step, comm, NULL);
    for(int i=0; i<n; i++){
      total[i] += temp_vals[i];
    }
  }

  free(temp_vals);
} /* Allreduce_ring */