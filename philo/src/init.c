/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/05 19:15:18 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/26 14:13:15 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	allocation(t_data *data, int num_of_philos)
{
	data->philos = malloc (num_of_philos * sizeof(t_philo));
	if (!data->philos)
		return (destroy_all(data));
	data->threads = malloc(num_of_philos * sizeof(pthread_t));
	if (!data->threads)
		return (destroy_all(data));
	data->forks = malloc (num_of_philos * sizeof(t_mutex));
	if (!data->forks)
		return (destroy_all(data));
	return (1);
}

static int	init_forks(t_data *data, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (destroy_all(data));
		i++;
	}
	return (1);
}

static int	init_data(t_data *data, char **input, int num)
{
	data->num_of_philos = num;
	data->time_to_die = ft_atoi(input[2]);
	data->time_to_eat = ft_atoi(input[3]);
	data->time_to_sleep = ft_atoi(input[4]);
	data->meals_limit = -1;
	if (input[5])
		data->meals_limit = ft_atoi(input[5]);
	data->start_time = 0;
	if (data->start_time == (size_t)-1)
		return (-1);
	data->philos_ready = false;
	data->dead = false;
	if (pthread_mutex_init(&data->dead_lock, NULL) != 0)
		return (destroy_all(data));
	if (pthread_mutex_init(&data->start_lock, NULL) != 0)
		return (destroy_all(data));
	if (pthread_mutex_init(&data->eat_lock, NULL) != 0)
		return (destroy_all(data));
	if (pthread_mutex_init(&data->write_lock, NULL) != 0)
		return (destroy_all(data));
	return (1);
}

static void	init_philos(t_data *data, char **input)
{
	int	i;
	int	num_of_philos;

	num_of_philos = ft_atoi(input[1]);
	i = 0;
	while (i < num_of_philos)
	{
		data->philos[i].data = data;
		data->philos[i].num = i + 1;
		data->philos[i].meals_eaten = 0;
		if (data->philos[i].num % 2 == 0)
			data->philos[i].type = 0;
		else
			data->philos[i].type = 1;
		data->philos[i].last_meal = get_time();
		data->philos[i].fork_left = &data->forks[i];
		data->philos[i].fork_right = &data->forks[(i + 1) % num_of_philos];
		i++;
	}
}

int	init(t_data *data, char **input)
{
	int	num_of_philos;

	num_of_philos = ft_atoi(input[1]);
	if (!allocation(data, num_of_philos))
		return (0);
	if (!init_forks(data, num_of_philos))
		return (0);
	if (!init_data(data, input, num_of_philos))
		return (0);
	init_philos(data, input);
	return (1);
}
