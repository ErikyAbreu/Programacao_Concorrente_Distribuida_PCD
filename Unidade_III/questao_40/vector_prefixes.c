/* File:     vector_prefixes.c
 *
 * Purpose:  Implement vector prefixes
 *
 * Compile:  gcc -g -Wall -o vector_prefixes vector_prefixes.c
 * Run:      ./vector_prefixes
 */
#include <stdio.h>
#include <stdlib.h>

void Read_n(int* n_p);
void Allocate_vector(double** x_pp, int n);
void Read_vector(double a[], int n, char vec_name[]);
void Print_vector(double b[], int n, char title[]);
void Element_prefixes(double d[], double c[], int n);

/*---------------------------------------------------------------------*/
int main(void) {
   int n;
   double *vector_x, *vector_result;

   Read_n(&n);
   Allocate_vector(&vector_x, n);
   Read_vector(vector_x, n, "x");
   Print_vector(vector_x, n, "VECTOR X:");

   Allocate_vector(&vector_result, n);
   for(int i=0; i < n; i++){
      Element_prefixes(vector_result, vector_x, i);
   }

   Print_vector(vector_result, n, "VECTOR RESULT:");

   free(vector_x);
   free(vector_result);

   return 0;
}  /* main */

//Read_n ---------------------------------------------------------------------
void Read_n(int* n_p /* out */) {
   printf("What's the order of the vectors?\n");
   scanf("%d", n_p);
   if (*n_p <= 0) {
      fprintf(stderr, "Order should be positive\n");
      exit(-1);
   }
}  /* Read_n */

//Allocate_vector ---------------------------------------------------------------------
void Allocate_vector(
      double**  x_pp  /* out */, 
      int       n     /* in  */) {
   *x_pp = malloc(n*sizeof(double));
   if (*x_pp == NULL) {
      fprintf(stderr, "Can't allocate vectors\n");
      exit(-1);
   }
}  /* Allocate_vectors */

//Read_vector ---------------------------------------------------------------------
void Read_vector(
      double  a[]         /* out */, 
      int     n           /* in  */, 
      char    vec_name[]  /* in  */) {
   int i;
   printf("Enter the vector %s\n", vec_name);
   for (i = 0; i < n; i++)
      scanf("%lf", &a[i]);
}  /* Read_vector */  

//Print_vector ---------------------------------------------------------------------
void Print_vector(
      double  b[]     /* in */, 
      int     n       /* in */, 
      char    title[] /* in */) {
   int i;
   printf("%s\n", title);
   for (i = 0; i < n; i++)
      printf("%.2f ", b[i]);
   printf("\n");
}  /* Print_vector */

//Element_prefixes ---------------------------------------------------------------------
void Element_prefixes(
      double  d[]     /* out */,
      double  c[]     /* in */,
      int     n       /* in */) {
   d[n] = 0;
   for(int i = 0; i <= n; i++){
      d[n] += c[i];
   }
}