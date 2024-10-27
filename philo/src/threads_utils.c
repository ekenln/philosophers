/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/17 17:27:32 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/27 17:32:30 by eeklund       ########   odam.nl         */
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
		pthread_mutex_unlock(&philos->data->eat_lock);
		current_time = get_time();
		if (current_time - last_meal >= philos->data->time_to_die)
		{
			mutx_bool(&philos->data->dead_lock, &philos->data->dead, true);
			pthread_mutex_lock(&philos->data->write_lock);
			timestamp = current_time - philos->data->start_time;
			printf("%lu %i %s\n", timestamp, philos[i].num, DIED);
			pthread_mutex_unlock(&philos->data->write_lock);
			return (1);
		}
		i++;
	}
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
		if (data->meals_limit != -1 && \
		data->philos[i].meals_eaten < data->meals_limit)
			fed = 0;
		pthread_mutex_unlock(&data->eat_lock);
		i++;
	}
	return (fed);
}

void	handle_one_philo(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	philo->data->start_time = get_time();
	philo->last_meal = philo->data->start_time;
	pthread_mutex_lock(philo->fork_left);
	print_message(TAKE_FORKS, philo);
	ft_usleep(philo->data->time_to_die);
	pthread_mutex_unlock(philo->fork_left);
	print_message(DIED, philo);
}

void	end_simulation(t_data *data, int threads_created)
{
	int	i;

	i = 0;
	while (i < threads_created)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
}
