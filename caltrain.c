#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	station->passengers_on_station = 0;
	station->passengers_on_train = 0;
	station->train_seats = 0;
	pthread_mutex_init(&station->lock, NULL);
	pthread_cond_init(&station->load_train, NULL);
	pthread_cond_init(&station->wait_train, NULL);
}

void
station_load_train(struct station *station, int count)
{
	station->available_seats = count;
	station->train_seats = count;
	if (station->train_seats && station->passengers_on_station)
	{
		pthread_cond_broadcast(&station->wait_train);
	}
	while (station->passengers_on_train < station->train_seats && station->passengers_on_station)
	{
		pthread_cond_wait(&station->load_train, &station->lock);
	}
	station->available_seats = 0;
	station->passengers_on_train = 0;
}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&station->lock);
	station->passengers_on_station ++;
	while (station->passengers_on_station && !station->available_seats)
	{
		pthread_cond_wait(&station->wait_train, &station->lock);
	}
	if (station->available_seats > 0)
	{
		station->available_seats --;
	}
	pthread_mutex_unlock(&station->lock);
}

void
station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->lock);
	station->passengers_on_station --;
	station->passengers_on_train ++;
	if (station->train_seats == station->passengers_on_train || !station->passengers_on_station)
	{
		pthread_cond_signal(&station->load_train);
	}
	pthread_mutex_unlock(&station->lock);
}
