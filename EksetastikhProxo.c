#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define TOTAL_PAIRS 5

sem_t partner_available;
sem_t expert_available;

void* expert(void* arg) {
    int id = *(int*)arg;
    int delay = rand() % 3 + 1;
    sleep(delay);

    printf("Expert %d arrived after %d sec and is looking for a partner...\n", id, delay);
    sem_post(&expert_available);  // Δηλώνει ότι είναι διαθέσιμος

    // Περιμένει να βρει διαθέσιμο partner
    if (sem_trywait(&partner_available) == 0) {  
        printf("Expert %d found a partner and is waiting to dance✅...\n", id);
        sem_wait(&expert_available);  // ✅ Περιμένει μέχρι να ξεκινήσει ο χορός
        printf("Expert %d is dancing now!\n", id);
        sleep(2);
        printf("Expert %d finished dancing and leaves.\n", id);
    } 

    return NULL;
}

void* partner(void* arg) {
    int id = *(int*)arg;
    int delay = rand() % 3 + 1;
    sleep(delay);

    printf("Partner %d arrived after %d sec and is looking for an expert...\n", id, delay);
    sem_post(&partner_available);  // Δηλώνει ότι είναι διαθέσιμος

    // Περιμένει να βρει διαθέσιμο expert
    if (sem_trywait(&expert_available) == 0) {  
        printf("Partner %d found an expert and is waiting to dance...\n", id);
        sem_wait(&partner_available);  // ✅ Περιμένει μέχρι να ξεκινήσει ο χορός
        printf("Partner %d is dancing now!\n", id);
        sleep(2);
        printf("Partner %d finished dancing and leaves.\n", id);
    } 

    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t experts[TOTAL_PAIRS], partners[TOTAL_PAIRS];
    int ids[TOTAL_PAIRS];

    sem_init(&partner_available, 0, 0);
    sem_init(&expert_available, 0, 0);

    for (int i = 0; i < TOTAL_PAIRS; i++) {
        ids[i] = i + 1;
        pthread_create(&experts[i], NULL, expert, &ids[i]);
        pthread_create(&partners[i], NULL, partner, &ids[i]);
    }

    for (int i = 0; i < TOTAL_PAIRS; i++) {
        pthread_join(experts[i], NULL);
        pthread_join(partners[i], NULL);
    }

    sem_destroy(&partner_available);
    sem_destroy(&expert_available);

    return 0;
}
