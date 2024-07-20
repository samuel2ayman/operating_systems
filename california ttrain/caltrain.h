#include <pthread.h>
struct station {
	int empty_seats;
	int waiting_passengers;
	int on_board_passengers;
	pthread_mutex_t train_lock;
    pthread_cond_t arrive_of_train;
	pthread_cond_t all_passengers_on_board;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);