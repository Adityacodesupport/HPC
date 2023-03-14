#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "omp.h"

#define MAX_SIZE 10

//Function for creating an input array||Update accoording to your need
void generate_list(int * x, int n) {
   int i,j,t;
   for (i = 0; i < n; i++)
     x[i] = i;
   for (i = 0; i < n; i++) {
     j = rand() % n;
     t = x[i];
     x[i] = x[j];
     x[j] = t;
   }
}

void print_list(int * x, int n) {
   int i;
   for (i = 0; i < n; i++) {
      printf("%d ",x[i]);
   }
}

//Merging 2 sorted subarrays into one tmp array
void merge(int * X, int n, int * tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;
	//i will iterate till first  half anf J will iterate for 2nd half of array
   while (i<n/2 && j<n) {
      if (X[i] < X[j]) {
         tmp[ti] = X[i];
         ti++; i++;
      } else {
         tmp[ti] = X[j];
         ti++; 
	 j++;
      }
   }
   while (i<n/2) { /* finish up lower half */
      tmp[ti] = X[i];
	ti++;
	i++;
   }
      while (j<n) { /* finish up upper half */
         tmp[ti] = X[j];
         ti++; 
	 j++;
   }
	//Copy sorted array tmp back to  X (Original array)
   memcpy(X, tmp, n*sizeof(int));

} // end of merge()

void mergesort(int * X, int n, int * tmp)
{
   if (n < 2) return;

   #pragma omp task firstprivate (X, n, tmp)
   mergesort(X, n/2, tmp);

   #pragma omp task firstprivate (X, n, tmp)
   mergesort(X+(n/2), n-(n/2), tmp);
	
   //Wait for both paralel tasks to complete execution
   #pragma omp taskwait

    /* merge sorted halves into sorted list */
   merge(X, n, tmp);
}


int main()
{
   int n = 10;
   double start, stop , end;

   int data[MAX_SIZE], tmp[MAX_SIZE];

   generate_list(data, n);
   printf("List Before merge Sorting...\n");
   print_list(data, n);
   start = omp_get_wtime();
   #pragma omp parallel
   {
      #pragma omp single
      mergesort(data, n, tmp);
   }
   stop = omp_get_wtime();
   printf("\nList After merge Sorting...\n");
   print_list(data, n);
   printf("\nTime: %g\n",stop-start);

 printf("\nList After bubble Sorting...\n");

	int SIZE =1<<4;
	int A[SIZE];
	for(int i=0;i<SIZE;i++)
	{
	    A[i]=rand()%SIZE;
	}
	//int A[5] = {6,9,1,3,7};
	int N = SIZE;
	int i=0, j=0; 
	int first;
	
	start=omp_get_wtime();
	for( i = 0; i < N-1; i++ )
	{
		first = i % 2; 
		#pragma omp parallel for default(none),shared(A,first,N)
		for( j = first; j < N-1; j += 1 )
		{
			if( A[ j ] > A[ j+1 ] )
			{       
                                int temp = A[j];
				A[j] =  A[j+1];
				A[j+1] = temp;
				//swap( &A[ j ], &A[ j+1 ] );
			}
		}
	}
end=omp_get_wtime();
	for(i=0;i<N;i++)
	{
		printf(" %d",A[i]);
	}

printf("\n-------------------------\n Time Parallel= %f",(end-start));
}

/*void swap(int *num1, int *num2)
{

	int temp = *num1;
	*num1 =  *num2;
	*num2 = temp;
}*/





