#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define BUFFER_SIZE 20
#define WORD_LEN 50
#define THREADS 3

char buffer[BUFFER_SIZE][WORD_LEN];
char result[BUFFER_SIZE][WORD_LEN];

int count = 0;
int result_count = 0;
int finished = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;
char *dictionary[] = {"hello","hi","cat","dog","tree","book","value"};
int dict_size = 7;
int check_dictionary(char *word) {
    for (int i = 0; i < dict_size; i++) {
        if (strcmp(dictionary[i], word) == 0)
            return 1;
    }
    return 0;
}
void *spell_check(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (count == 0 && !finished) {
            pthread_cond_wait(&cond, &mutex);
        }
        if (count == 0 && finished) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        char word[WORD_LEN];
        strcpy(word, buffer[count - 1]);
        count--;

        pthread_mutex_unlock(&mutex);

        int status = check_dictionary(word);

        pthread_mutex_lock(&mutex);
        if (status) {
            sprintf(result[result_count++], "%s is Correct", word);
        } else {
            sprintf(result[result_count++], "%s is  Incorrect", word);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t threads[THREADS];
    int n;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    printf("Enter number of words : ");
    scanf("%d", &n);
    if (n > BUFFER_SIZE) {
        n = BUFFER_SIZE;
    }

    printf("Enter words:");
    for (int i = 0; i < n; i++) {
        scanf("%s", buffer[i]);
    }

    count = n;
    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, spell_check, NULL);
    }
    pthread_cond_broadcast(&cond);
    pthread_mutex_lock(&mutex);
    finished = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nOutput:\n");
    for (int i = 0; i < result_count; i++) {
        printf("%s\n", result[i]);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
