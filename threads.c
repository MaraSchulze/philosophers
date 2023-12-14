/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marschul <marschul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:06:02 by marschul          #+#    #+#             */
/*   Updated: 2023/12/12 21:44:50 by marschul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	start_threads(t_main_data *main_data, pthread_t *threads, t_data *data)
{
	int	i;

	printf("in start threads\n");
	i = 0;
	while (i < main_data->nr_philosophers)
	{
		debug_print(data);
		if (pthread_create(&threads[i], NULL, philosopher, (void*) &data[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

void	join_threads(t_main_data *main_data, pthread_t *threads)
{
	int	i;

	printf("in join threads\n");
	i = 0;
	while (i < main_data->nr_philosophers)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	printf("exited");
}