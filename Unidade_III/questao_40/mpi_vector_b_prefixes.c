/* File:     vector_prefixes.c
 *
 * Purpose:  Implement vector prefixes
 *
 * Compile:  mpicc -g -Wall -o mpi_vector_b_prefixes mpi_vector_b_prefixes.c
 * Usage:    mpiexec -n<number of processes> ./mpi_vector_b_prefixes
 */
#include <stdio.h>
#include <mpi.h> 
#include <stdlib.h>

void Check_for_error(int local_ok, char fname[], char message[], MPI_Comm comm);
void Read_n(int* n_p, int my_rank, int comm_sz, MPI_Comm comm);
void Allocate_vector(double** x_pp, int n, MPI_Comm comm);
void Read_vector(double a[], int n, char vec_name[], int my_rank, MPI_Comm comm);

/*---------------------------------------------------------------------*/
int main(void) {
   int n_total, comm_sz, my_rank, i;
   double *vector_x, *vector_result;
   double location = 0;

   MPI_Init(NULL, NULL); 
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   Read_n(&n_total, my_rank, comm_sz, MPI_COMM_WORLD);
   Allocate_vector(&vector_x, n_total, MPI_COMM_WORLD);
   Read_vector(vector_x, n_total, "VECTOR X", my_rank, MPI_COMM_WORLD);
   for(i = 0; i<=my_rank; i++){
      location += vector_x[i];
   }
   Allocate_vector(&vector_result, n_total, MPI_COMM_WORLD);

   if(my_rank==0){
      vector_result[0] = location;
      for(i = 1; i < n_total; i++){
      MPI_Recv(&vector_result[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, NULL);
      }
   }else{
      MPI_Send(&location, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
   }

   if(my_rank==0){
      printf("VECTOR RESULT: ");
      for (i = 0; i < n_total; i++)
      {
         printf("%.2f ", vector_result[i]);
      }
   }
   printf("\n");
   free(vector_result);
   free(vector_x);
   MPI_Finalize();
   return 0;
}  /* main */

// Check_for_error -------------------------------------------------------------------

void Check_for_error(
      int       local_ok   /* in */, 
      char      fname[]    /* in */,
      char      message[]  /* in */, 
      MPI_Comm  comm       /* in */) {
   int ok;

   MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
   if (ok == 0) {
      int my_rank;
      MPI_Comm_rank(comm, &my_rank);
      if (my_rank == 0) {
         fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, fname, 
               message);
         fflush(stderr);
      }
      MPI_Finalize();
      exit(-1);
   }
}  /* Check_for_error */

//Read_n ---------------------------------------------------------------------
void Read_n(
      int*      n_p        /* out */, 
      int       my_rank    /* in  */, 
      int       comm_sz    /* in  */,
      MPI_Comm  comm       /* in  */) {
   int local_ok = 1;
   char *fname = "Read_n";
   
   if (my_rank == 0) {
      printf("What's the order of the vectors?\n");
      scanf("%d", n_p);
   }
   MPI_Bcast(n_p, 1, MPI_INT, 0, comm);
   if (*n_p <= 0 || *n_p != comm_sz) local_ok = 0;
   Check_for_error(local_ok, fname, "n should be > 0 and evenly divisible by comm_sz", comm);
}  /* Read_n */

//Allocate_vector ---------------------------------------------------------------------

void Allocate_vector(
      double**   x_pp  /* out */, 
      int        n     /* in  */,
      MPI_Comm   comm  /* in  */) {
   int local_ok = 1;
   char* fname = "Allocate_vector";

   *x_pp = malloc(n*sizeof(double));

   if (*x_pp == NULL) local_ok = 0;
   Check_for_error(local_ok, fname, "Can't allocate local vector(s)", comm);
}  /* Allocate_vectors */

//Read_vector ---------------------------------------------------------------------

void Read_vector(
      double    a[]         /* out */, 
      int       n           /* in  */,
      char      vec_name[]  /* in  */,
      int       my_rank     /* in  */, 
      MPI_Comm  comm        /* in  */) {

   int i;
   int local_ok = 1;
   char* fname = "Read_vector";
   
   if (a == NULL) local_ok = 0;
   Check_for_error(local_ok, fname, "Vector not allocated", comm);

   if (my_rank == 0) {
      printf("Enter the vector %s\n", vec_name);
      for (i = 0; i < n; i++){
         scanf("%lf", &a[i]);
      }
   } 
   MPI_Bcast(a, n, MPI_DOUBLE, 0, comm);
}  /* Read_vector */ 