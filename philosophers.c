/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marschul <marschul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 13:36:21 by marschul          #+#    #+#             */
/*   Updated: 2023/12/15 22:53:12 by marschul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/time.h>
#include <unistd.h>

int	get_ms(struct timeval start_time, struct timeval timestamp)
{
	int	diff_sec;
	int	diff_usec;
	int	diff;

	diff_sec = (timestamp.tv_sec - start_time.tv_sec) * 1000;
	diff_usec = (timestamp.tv_usec - start_time.tv_usec) / 1000;
	diff = diff_sec + diff_usec;
	return (diff);
}

void	do_log(struct timeval timestamp, t_data *data, char *text)
{
	pthread_mutex_lock(data->logger);
	printf("%d %d %s\n", get_ms(data->start_time, timestamp), data->number, text);
	pthread_mutex_unlock(data->logger);
}

int	die(int	dying_time, t_data *data)
{
	struct timeval	timestamp;

	printf("%d in die\n", data->number);

	// pthread_mutex_lock(data->logger);
	// printf("begin %d : ", data->number);
	// for (int i = 0; i < 30; i++)
	// {
	// 	pthread_mutex_lock(&data->forks[i].fork_lock);
	// 	printf("%d ", data->forks[i].fork);
	// 	pthread_mutex_lock(&data->forks[i].fork_lock);

	// }
	// printf("end \n");
	// pthread_mutex_unlock(data->logger);
	printf("dying time %d %d\n", data->number, dying_time);
	assert(dying_time > 0 && dying_time <= data->time_to_die);
	usleep(dying_time * 1000);

	if (gettimeofday(&timestamp, NULL) != 0)
		return (0);
	do_log(timestamp, data, "died");


	
	return (0);
}

int	try_forks(t_data *data, int left)
{
	int	right;
	
		printf("Trying %d\n : ", data->number);

	// pthread_mutex_lock(data->logger);
	// printf("Trying %d : ", data->number);
	// // for (int i = 0; i < data->nr_philosophers; i++)
	// // {
	// // 	pthread_mutex_lock(&data->forks[i].fork_lock);
	// // 	printf("%d ", data->forks[i].fork);
	// // 	pthread_mutex_unlock(&data->forks[i].fork_lock);
	// // }
	// printf("\n");
	// pthread_mutex_unlock(data->logger);
		printf("Trying 2%d\n : ", data->number);

	right = data->number % data->nr_philosophers;
	pthread_mutex_lock(&data->forks[left].fork_lock);
	if (data->forks[left].fork == 0)
	{
		data->forks[left].fork = data->number;
		pthread_mutex_unlock(&data->forks[left].fork_lock);
		pthread_mutex_lock(&data->forks[right].fork_lock);
		if (data->forks[right].fork == 0)
		{
			data->forks[right].fork = data->number;
			pthread_mutex_unlock(&data->forks[right].fork_lock);
			return (1);
		}
		else
		{
			printf("------- %d is disappointed\n", data->number);
			pthread_mutex_unlock(&data->forks[right].fork_lock);
			pthread_mutex_lock(&data->forks[left].fork_lock);
			assert(data->forks[left].fork == data->number);
			data->forks[left].fork = 0;
			pthread_mutex_unlock(&data->forks[left].fork_lock);
			return (0);
		}
	}
	else
	{
		pthread_mutex_unlock(&data->forks[left].fork_lock);
		return (0);
	}
}

int	take_forks(t_data *data)
{
	int				success;
	struct timeval	timestamp;

	success = 0;
	while (success != 1)
	{	
		success = try_forks(data, data->number - 1);
		if (success == 0)
			usleep(data->number * 10);
	}
	if (gettimeofday(&timestamp, NULL) != 0)
		return (0);
	do_log(timestamp, data, "has taken two forks");
	return (1);
}

int	eat(t_data *data)
{
	struct timeval	timestamp;
	int				left;
	int				right;

		printf("%d in eat\n", data->number);

	if (gettimeofday(&timestamp, NULL) != 0)
		return (0);
	left = data->number - 1;
	right = data->number % data->nr_philosophers;
	if (get_ms(data->eat_time, timestamp) + data->time_to_eat > data->time_to_die)
	{
		printf("%d here\n", data->number);

		pthread_mutex_lock(&data->forks[left].fork_lock);
		data->forks[left].fork = 0;
		pthread_mutex_unlock(&data->forks[left].fork_lock);
		printf("%d here\n", data->number);

		pthread_mutex_lock(&data->forks[right].fork_lock);
		data->forks[right].fork = 0;
		pthread_mutex_unlock(&data->forks[right].fork_lock);
		printf("%d here\n", data->number);
		return (die(data->time_to_die - (get_ms(data->eat_time, timestamp) + data->time_to_eat), data));
	}
	do_log(timestamp, data, "is eating");
	usleep(data->time_to_eat * 1000);
	pthread_mutex_lock(&data->forks[left].fork_lock);
	data->forks[left].fork = 0;
	pthread_mutex_unlock(&data->forks[left].fork_lock);
	pthread_mutex_lock(&data->forks[right].fork_lock);
	data->forks[right].fork = 0;
	pthread_mutex_unlock(&data->forks[right].fork_lock);
	return (1);
}

int	do_sleep(t_data *data)
{
	struct timeval	timestamp;

	if (gettimeofday(&timestamp, NULL) != 0)
		return (0);
	data->eat_time = timestamp;
	if (data->time_to_eat > data->time_to_die)
		return (die(data->time_to_die, data));
	do_log(timestamp, data, "is sleeping");
	usleep(data->time_to_eat * 1000);
	do_log(timestamp, data, "is thinking");
	return (1);
}

void	*philosopher(void *data)
{
	t_data 			*this_data;
	int				loops;

	this_data = (t_data *) data;
	loops = this_data->nr_meals;
	while (loops != 0)
	{
		if (take_forks(this_data) == 0)
			return (NULL);
		if (eat(this_data) == 0)
			return (NULL);
		if (do_sleep(this_data) == 0)
			return (NULL);
		loops--;
	}
	return (NULL);
}