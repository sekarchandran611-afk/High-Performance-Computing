#include <stdio.h>
#include <omp.h>

int main() {
    int i, n = 10;
    int a[10];

    for(i = 0; i < n; i++) {
        a[i] = i * 2;
   }

    double start = omp_get_wtime();

    # pragma omp parallel for schedule(static, 2)
    for(i = 0; i < n; i++) {
        printf("Thread %d is processing element %d\n", omp_get_thread_num(), i);
        a[i] = a[i] + 5;
    }
    double end = omp_get_wtime();

    printf("Time:%fsec \n",end-start);

    printf("\nFinal Array:\n");
    for(i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }

    return 0;
}
