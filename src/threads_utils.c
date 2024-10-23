/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/17 17:27:32 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/23 19:22:39 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_is_dead(t_philo *philos)
{
	int		i;
	size_t	last_meal;
	size_t	current_time;
	size_t	timestamp;

	i = 0;
	while (i < philos->data->num_of_philos)
	{
		pthread_mutex_lock(&philos->data->eat_lock);
		last_meal = philos[i].last_meal;
		current_time = get_time();
		// printf("philo num: %i, cur - last_meal %zu\n", philos[i].num, current_time - last_meal);
		pthread_mutex_unlock(&philos->data->eat_lock);
		if (current_time - last_meal >= philos->data->time_to_die)
		{
			pthread_mutex_lock(&philos->data->dead_lock);
			philos->data->dead = true;
			pthread_mutex_unlock(&philos->data->dead_lock);
			timestamp = current_time - philos->data->start_time;
			print_safe(philos, timestamp, philos[i].num, DIED);
			return (1);
		}
		i++;
	}
	// ft_usleep(100);
	return (0);
}

int	all_philos_alive(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->dead_lock);
	if (philo->data->dead == true)
		return (pthread_mutex_unlock(&philo->data->dead_lock), 0);
	return (pthread_mutex_unlock(&philo->data->dead_lock), 1);
}

int	all_fed(t_data *data)
{
	int	i;
	int	fed;

	i = 0;
	fed = 1;
	while (i < data->num_of_philos)
	{
		pthread_mutex_lock(&data->eat_lock);
		if (data->meals_limit && data->philos[i].meals_eaten < data->meals_limit)
			fed = 0;
		pthread_mutex_unlock(&data->eat_lock);
		i++;
	}
	return (fed);
}
