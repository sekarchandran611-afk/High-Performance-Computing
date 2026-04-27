#define _POSIX_C_SOURCE 199309L
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define N 5  // You can change matrix size here (e.g., 5, 50, 100)

// Function to generate random matrix
void generateMatrix(int n, int **A) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = rand() % 10;
}

// Print int ** matrix
void printMatrix(int n, int **A) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", A[i][j]);
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    int n;
    printf("Enter matrix limit:");
    scanf("%d",&n);

    // Allocate matrices dynamically
    int **matA = malloc(n * sizeof(int *));
    int **matB = malloc(n * sizeof(int *));
    int **mat_add = malloc(n * sizeof(int *));
    int **mat_sub = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matA[i] = malloc(n * sizeof(int));
        matB[i] = malloc(n * sizeof(int));
        mat_add[i] = malloc(n * sizeof(int));
        mat_sub[i] = malloc(n * sizeof(int));
    }

    printf("Matrix size: %dx%d\n\n", n, n);

    generateMatrix(n, matA);
    generateMatrix(n, matB);

    if (n <= 5) {
        printf("Matrix A:\n"); printMatrix(n, matA);
        printf("\nMatrix B:\n"); printMatrix(n, matB);
    }

    struct timespec start, end;

    // ================= SERIAL EXECUTION =================
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            mat_add[i][j] = matA[i][j] + matB[i][j];

            if (n <= 5)
                printf("Serial: add[%d][%d]=%d\n",
                       i,j,mat_add[i][j]);
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            mat_sub[i][j] = matA[i][j] - matB[i][j];

            if (n <= 5)
                printf("Serial: sub[%d][%d]=%d\n",
                       i,j,mat_sub[i][j]);
        }
    }


    clock_gettime(CLOCK_MONOTONIC, &end);
    double serial_time =
        (end.tv_sec - start.tv_sec)*1000 +
        (end.tv_nsec - start.tv_nsec)/1000000;

    if (n <= 5) {
        printf("\nSerial Addition Result:\n"); printMatrix(n, mat_add);
        printf("\nSerial Subtraction Result:\n"); printMatrix(n, mat_sub);
    }

    // ================= PARALLEL EXECUTION =================

    double pll_time=0,parallel_time=0,pal_time=0;
    pid_t pid = fork();
    pll_time=0;
    if (pid == 0) {
        // -------- CHILD PROCESS: subtraction --------
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                mat_sub[i][j] = matA[i][j] - matB[i][j];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        pll_time =(end.tv_sec - start.tv_sec)*1000 +(end.tv_nsec - start.tv_nsec)/1000000;
        if(pll_time>parallel_time){
                parallel_time=pll_time;
        }
        if (n <= 5) {
            printf("\nChild Process: Subtraction Result:\n");
            printMatrix(n, mat_sub);
        }
        exit(0);
    } else {
        // -------- PARENT PROCESS: addition --------
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                mat_add[i][j] = matA[i][j] + matB[i][j];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        pal_time =(end.tv_sec - start.tv_sec)*1000 +(end.tv_nsec - start.tv_nsec)/1000000;
        if(pal_time>parallel_time){
                parallel_time=pal_time;
        }
        if (n <= 5) {
            printf("\nParent Process: Addition Result:\n");
            printMatrix(n, mat_add);
        }
        wait(NULL);
    }


    printf("\nExecution Time:\n");
    printf("Serial Time   : %lf Milliseconds\n", serial_time);
    printf("Parallel Time : %lf Milliseconds\n", parallel_time);

    // Free memory
  for (int i = 0; i < n; i++) {
        free(matA[i]);
        free(matB[i]);
        free(mat_add[i]);
        free(mat_sub[i]);
    }
    free(matA);
    free(matB);
    free(mat_add);
    free(mat_sub);

    return 0;
}
