#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include<time.h>

#define MAX 100

int pal(char str[]) {
    char rev[MAX];
    int len = strlen(str);
    for (int i = 0; i < len; i++)
        rev[i] = str[len - i - 1];
    rev[len] = '\0';

    return strcmp(str, rev) == 0;
}

int main(int argc, char *argv[]) {
    int rank, size;
    char msg[MAX];
    MPI_Status status;
    clock_t start_time, end_time;
    float exec_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank != 0) {
        if(rank==1)
                strcpy(msg,"hello");
        else if (rank == 2)
                strcpy(msg,"madam");
        else if(rank==3)
                strcpy(msg,"work");
        else if(rank==4)
                strcpy(msg,"siva");
        else if(rank==5)
                strcpy(msg,"letter");
        else if(rank=6)
                strcpy(msg,"radar");
        else if(rank=7)
                strcpy(msg,"book");

        start_time = clock();

        if (rank % 2 == 1) {
            MPI_Send(msg, MAX, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        } else {
            MPI_Send(msg, MAX, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
        }
        end_time = clock();
        exec_time = ((double)(end_time - start_time) / CLOCKS_PER_SEC);
        printf("Execution time of process %d :  %lf\n",rank,exec_time);
    } else {
        for (int i = 1; i < size; i++) {
            MPI_Recv(msg, MAX, MPI_CHAR,MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (status.MPI_TAG == 1) {
                for (int k = 0; msg[k] != '\0'; k++) {
                     msg[k] = toupper(msg[k]);
                }
                printf("Process %d | Type 1 | Uppercase: %s\n", status.MPI_SOURCE, msg);
            } else if (status.MPI_TAG == 2) {
                if (pal(msg)) {
                    printf("Process %d | Type 2 | %s is a palindrome\n", status.MPI_SOURCE, msg);
                } else {
                    printf("Process %d | Type 2 | %s is not a palindrome\n", status.MPI_SOURCE, msg);
                }
            }
        }
    }

    MPI_Finalize();
    return 0;
}
