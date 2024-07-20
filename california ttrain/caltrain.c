#include <pthread.h>
#include "caltrain.h"
void station_init(struct station *station)
{
	station->empty_seats=0;
    station->waiting_passengers=0;
    station->on_board_passengers=0;
    pthread_cond_init(&(station->arrive_of_train),NULL);
    pthread_cond_init(&(station->all_passengers_on_board),NULL);
    pthread_mutex_init(&(station->train_lock),NULL);

}

void station_load_train(struct station *station, int count)
{
    
	pthread_mutex_lock(&(station->train_lock)); //enter critical section
    if(!count || !station->waiting_passengers)
    {
        pthread_mutex_unlock(&(station->train_lock));//leave critical section
        return;
    }
    station->empty_seats=count;
    pthread_cond_broadcast(&(station->arrive_of_train));//alarm arrival of train for all passengers
    pthread_cond_wait(&(station->all_passengers_on_board), &(station->train_lock));//wait for all passengers to get on board
    station->empty_seats=0;
    pthread_mutex_unlock(&(station->train_lock));//leave critical section
}

void station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&(station->train_lock));//enter critical section
    station->waiting_passengers++; //increase no of waiting passengers
    while( station->empty_seats==0) //full train wait for another
    {
        pthread_cond_wait(&(station->arrive_of_train),&(station->train_lock));
    }
    station->waiting_passengers--;
    station->empty_seats--; //reserve seat for passenger gone in board
    station->on_board_passengers++;
    pthread_mutex_unlock(&(station->train_lock));//leave critical section
}

void station_on_board(struct station *station)
{
	pthread_mutex_lock(&(station->train_lock));//enter critical section
    station->on_board_passengers --;
    //all on board passengers are seated and (no empty seats or no waiting passengers)
    if (station->on_board_passengers == 0 &&((station->empty_seats == 0) ||(station->waiting_passengers== 0)))
    {pthread_cond_signal(&(station->all_passengers_on_board));} //signal to train to leave
    pthread_mutex_unlock(&(station->train_lock));//leave critical section
}
//gcc caltrain.c caltrain_runner.c -o caltrain -pthread
//./caltrain