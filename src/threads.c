/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 16:00:54 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/20 12:45:35 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor(t_data *data)
{
	data->start_time = get_time();
	apply_mutx_bool(&data->start_lock, &data->philos_ready, true);
	while (1)
	{
		if ((data->meals_limit && all_fed(data)) || philo_is_dead(data->philos))
			return ;
	}
}

void	routine(void *philo_ptr)
{
	t_philo	*philo;
	bool	ready;

	philo = (t_philo *)philo_ptr;
	ready = false;
	while (ready == false)
	{
		apply_mutx_bool(&philo->data->start_lock, &ready, \
		philo->data->philos_ready);
	}
	apply_mutx(&philo->data->eat_lock, &philo->last_meal, \
	philo->data->start_time);
	while (all_philos_alive(philo))
	{
		if (philo->data->meals_limit && all_fed(philo->data))
			return ;
		print_message(THINKING, philo);
		if (eating(philo))
		{
			print_message(SLEEPING, philo);
			ft_usleep(philo->data->time_to_sleep);
		}
	}
}

int	start_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_create(&data->threads[i], NULL, (void *)routine, \
		(void *)&data->philos[i]) != 0)
			return (destroy_all(data));
		i++;
	}
	i = 0;
	monitor(data);
	while (i < data->num_of_philos)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	return (1);
}
