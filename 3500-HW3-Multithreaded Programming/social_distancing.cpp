/*
 * Hoang Do
 * social_distancing.cpp
 * 05/07/2021
 *
 * Description: This program is to allow vaccinated kids play together and exactly one non-vaccinated kids to play with them.
 */


#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int vaccinated_kids_count = 0;
sem_t _mutex;
sem_t play_mutex;

void Play(string type, void *threadID){
    printf("%s Thread %i playing! \n", type.c_str(), threadID);

    sleep(1);

    printf("%s Thread %i playing done! \n",type.c_str(), threadID);
}
//non-vaccinated threads
void* nvThread_Function(void *threadID){
    do{
        printf("NV Thread %i trying to acquire play_mutex \n", threadID);
        sem_wait(&play_mutex);
        Play("NV", threadID);
        sem_post(&play_mutex);
        sleep(1);
    } while(true);
}

//vaccinated threads
void* vThread_Function(void *threadID) {
    do {
        sem_wait(&_mutex);
        printf("V Thread %i acquired mutex for incrementing the count. \n", threadID);
        vaccinated_kids_count++;

        if (vaccinated_kids_count == 1) {
            printf("V Thread %i trying to acquire play_mutex. \n", threadID);
            sem_wait(&play_mutex);
        }
        printf("V Thread %i about to release mutex after incrementing. \n", threadID);
        sem_post(&_mutex);

        Play("V", threadID);

        sem_wait(&_mutex);
        printf("V Thread %i acquired mutex for decrementing the count. \n", threadID);
        vaccinated_kids_count--;
        if (vaccinated_kids_count == 0) {
            printf("V Thread %i about to release play_mutex. \n", threadID);
            sem_post(&play_mutex);
        }
        printf("V Thread %i about to release mutex after decrementing. \n", threadID);
        sem_post(&_mutex);
        sleep(1);
    } while (true);
}

int main(int argc, char* argv[]){
    //intialize semaphores
    sem_init(&_mutex, 0, 1);
    sem_init(&play_mutex, 0, 1);

    //if 3 arguments are not passed, throw an exception
    if(argc != 3){
        cerr << "Usage: " << argv[0] << "First number: ___  " << "  Second number: ___" << endl;
        cerr << "Please enter two numbers with a space in between" << endl;
        exit(3);
    }

    //data initialization
    int vSize = stoi(argv[1]);
    int nvSize = stoi(argv[2]);
    pthread_t vThread[vSize];
    pthread_t nvThread[nvSize];
    int v, nv;

    //create vaccinated threads
    for(int i = 0; i < vSize; i++){
        v = pthread_create(&vThread[i], NULL, vThread_Function, (void*) i);
        if(v){
            cout << "Error: unable to create thread, " << v << endl;
            exit(-1);

        }
    }

    //create non-vaccinated threads
    for(int i = 0; i < nvSize; i++){
        nv = pthread_create(&nvThread[i], NULL, nvThread_Function, (void*) i);
        if(nv){
            cout << "Error: unable to create thread, " << nv << endl;
            exit(-1);

        }
    }

    //join vaccinated threads
    for(int i = 0; i < vSize; i++){
        v = pthread_join(vThread[i], nullptr);
    }

    //join nonvaccinated threads
    for(int i = 0; i < nvSize; i++){
        nv = pthread_join(nvThread[i], nullptr);
    }

    pthread_exit(NULL);




}