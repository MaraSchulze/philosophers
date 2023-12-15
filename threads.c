/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marschul <marschul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 14:06:02 by marschul          #+#    #+#             */
/*   Updated: 2023/12/15 18:45:15 by marschul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	start_threads(t_main_data *main_data, pthread_t *threads, t_data *data)
{
	int	i;

	i = 0;
	while (i < main_data->nr_philosophers)
	{
		if (pthread_create(&threads[i], NULL, philosopher, (void*) &data[i]) != 0)
			return (0);
		//pthread_detach(threads[i]);
		i++;
	}
	return (1);
}

int	join_threads(t_main_data *main_data, pthread_t *threads)
{
	int	i;
	int	retval;
	int	*retval2 = &retval;

	i = 0;
	while (i < main_data->nr_philosophers)
	{
		if (pthread_join(threads[i], (void **) &retval2) != 0)
			return (0);
		printf("exited %d\n", i);
		i++;
	}
}