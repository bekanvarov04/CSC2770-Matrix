#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 3
#define NUM_THREADS 3

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];

int calculationCount = 0;
pthread_mutex_t mutex;

typedef struct {
    int row;
    int col;
} thread_data_t;

void *multiply(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        int product = matrixA[data->row][i] * matrixB[i][data->col];

        pthread_mutex_lock(&mutex); 
        resultMatrix[data->row][data->col] += product;
        calculationCount++; 
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MATRIX_SIZE * MATRIX_SIZE];
    thread_data_t thread_data[MATRIX_SIZE * MATRIX_SIZE];

    pthread_mutex_init(&mutex, NULL);

    // Initialize matrices A and B
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = rand() % 10;
            matrixB[i][j] = rand() % 10;
        }
    }

    // Create threads to compute the result matrix
    int thread_count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            thread_data[thread_count].row = i;
            thread_data[thread_count].col = j;
            pthread_create(&threads[thread_count], NULL, multiply, (void *)&thread_data[thread_count]);
            thread_count++;

            if (thread_count >= MATRIX_SIZE * MATRIX_SIZE)
                thread_count = 0;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { 
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    printf("Result Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }

    printf("Total calculations completed: %d\n", calculationCount);

    pthread_mutex_destroy(&mutex);

    return 0;
}