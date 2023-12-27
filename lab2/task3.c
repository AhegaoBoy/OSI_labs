#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 4

sem_t forks[NUM_PHILOSOPHERS];
sem_t mutex;

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;


    // Think
    printf("Philosopher %d is thinking\n", id);
    sleep(rand() % 3);

    // Pick up forks
    sem_wait(&mutex);
    sem_wait(&forks[left_fork]);
    sem_wait(&forks[right_fork]);
    sem_post(&mutex);

    // Eat
    printf("Philosopher %d is eating\n", id);
    sleep(rand() % 3);

    // Put down forks
    printf("Philosopher %d put forks\n", id);
    sem_post(&forks[left_fork]);
    sem_post(&forks[right_fork]);

}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}