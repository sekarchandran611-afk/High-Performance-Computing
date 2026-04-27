#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10
int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX], D[MAX][MAX];
int rows,cols;
int i,j,k;
void* addMatrices(void* arg);
void* multiplyMatrices(void* arg);

int main() {
    printf("Enter r and c for Matrix A: ");
    scanf("%d %d", &rows, &cols);
    printf("Enter  Matrix A:\n");
    for ( i = 0; i < rows; i++)
        for ( j = 0; j < cols; j++)
            scanf("%d", &A[i][j]);

    printf("Enter r and c for Matrix B: ");
    scanf("%d %d", &rows, &cols);
    printf("Enter Matrix B:\n");
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            scanf("%d", &B[i][j]);

    pthread_t addThread, mulThread;
    pthread_create(&addThread, NULL, addMatrices, NULL);

    pthread_create(&mulThread, NULL, multiplyMatrices, NULL);


    pthread_join(addThread, NULL);
    pthread_join(mulThread, NULL);


    printf("Addition:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    printf("Multiplication:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", D[i][j]);
        }
        printf("\n");
    }

    return 0;
}

// Function to add matrices
void* addMatrices(void* arg) {
    for (i = 0; i < rows; i++) {
        for ( j = 0; j < cols; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    pthread_exit(0);
}

// Function to multiply matrices
void* multiplyMatrices(void* arg) {
    for (i = 0; i < rows; i++) {
        for ( j = 0; j < cols; j++) {
            D[i][j] = 0; // Initialize the result cell
            for (k = 0; k < cols; k++) {
                D[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(0);
}
