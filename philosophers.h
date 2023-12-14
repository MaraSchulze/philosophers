/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marschul <marschul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 13:34:37 by marschul          #+#    #+#             */
/*   Updated: 2023/12/12 21:45:08 by marschul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

typedef struct s_forks {
	int				fork;
	pthread_mutex_t	fork_lock;
} 	t_forks;

typedef struct s_main_data {
	int	nr_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nr_meals;
	struct timeval start_time;
	pthread_mutex_t	logger;
	t_forks	*forks;
}	t_main_data;

typedef struct s_data {
	int	number;
	int	nr_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nr_meals;
	struct timeval start_time;
	struct timeval eat_time;
	pthread_mutex_t	*logger;
	t_forks	*forks;
}	t_data;

int		get_args(t_main_data *main_data, int argc, char **argv);
int		init_locks(t_main_data *main_data);
int		create_datastructures(t_main_data *main_data, pthread_t **threads, t_data **data);
int		start_threads(t_main_data *main_data, pthread_t *threads, t_data *data);
void	join_threads(t_main_data *main_data, pthread_t *threads);
void	*philosopher(void *data);
int		ft_atoi(const char *str);
void	debug_print(t_data *data);

#endif