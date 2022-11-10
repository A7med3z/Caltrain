#include <pthread.h>

struct station {
	int available_seats;
	int train_seats;
	int passengers_on_train;
	int passengers_on_station;
	pthread_mutex_t lock;
	pthread_cond_t wait_train;
	pthread_cond_t load_train;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
