/* File:    omp_26.c
 *
 * Compile: gcc -g -Wall -fopenmp -o omp_26 _copia omp_26 _copia.c
 * Usage:   ./omp_26 _copia <number of threads>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);

int main(int argc, char* argv[]) {
   int thread_count, n;
   double global_time;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter the value of n\n");
   scanf("%d", &n);


#  pragma omp parallel num_threads(thread_count) \
   default (none) shared(global_time, n)
      {
      double start, finish;

#     pragma omp barrier
      start = omp_get_wtime();
      int i, thread_number;
      double my_sum = 0.0;
      
      for (i = 1; i <= n; i++) {
#        pragma omp critical (sum)
            my_sum += sin(i);
      }
      thread_number = omp_get_thread_num();
      printf("Thread: %d  -  My sum: %.2f\n", thread_number, my_sum);
      finish = omp_get_wtime();
      printf("Thread %d Processing time: %f \n", thread_number, (finish-start));

#     pragma omp critical (time)
      {
      if(global_time < (finish-start)){
         global_time = (finish-start);
      }
      }
   }

   printf("Processing time: %f \n", global_time);

   return 0;
}  /* main */

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
      exit(0);
}  /* Usage */
