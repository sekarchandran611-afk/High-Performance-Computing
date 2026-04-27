#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n = 1000000;
    int *A, *B, *C;

    A = (int*)malloc(n * sizeof(int));
    B = (int*)malloc(n * sizeof(int));
    C = (int*)malloc(n * sizeof(int));

   for(int i = 0; i < n; i++) {
        A[i] = i;
        B[i] = i * 2;
    }

   double start = omp_get_wtime();

    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        C[i] = A[i] + B[i];
    }

    double end = omp_get_wtime();

    printf("Time:%fsec \n",end-start);

    printf("First 10 elements of result array:\n");
    for(int i = 10; i < 20; i++) {
        printf("%d\n ", C[i]);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}
