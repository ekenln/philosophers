/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 16:00:54 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/26 14:41:12 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	monitor(t_data *data)
{
	data->start_time = get_time();
	pthread_mutex_lock(&data->start_lock);
	data->philos_ready = true;
	pthread_mutex_unlock(&data->start_lock);
	while (1)
	{
		if (philo_is_dead(data->philos) || \
		(data->meals_limit != -1 && all_fed(data)))
			return ;
	}
}

static void	wait_for_start(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->start_lock);
		if (philo->data->philos_ready == true)
		{
			pthread_mutex_unlock(&philo->data->start_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->start_lock);
		usleep(5);
	}
}

static void	init_philosopher(t_philo *philo, int waiting_time)
{
	pthread_mutex_lock(&philo->data->eat_lock);
	philo->last_meal = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->eat_lock);
	print_message(THINKING, philo);
	if (philo->type == 0)
		ft_usleep(waiting_time);
}

static void	routine(void *philo_ptr)
{
	t_philo	*philo;
	int		waiting;

	philo = (t_philo *)philo_ptr;
	waiting = philo->data->time_to_eat / 2;
	wait_for_start(philo);
	init_philosopher(philo, waiting);
	while (all_philos_alive(philo))
	{
		if (philo->data->meals_limit != -1 && all_fed(philo->data))
			return ;
		eating(philo);
		if (print_message(SLEEPING, philo))
		{
			ft_usleep(philo->data->time_to_sleep);
			print_message(THINKING, philo);
		}
	}
}

int	start_threads(t_data *data)
{
	int	i;

	i = 0;
	if (data->num_of_philos == 1)
	{
		if (pthread_create(&data->threads[i], NULL, (void *)handle_one_philo, \
		(void *)&data->philos[i]) != 0)
			return (destroy_all(data));
		return (end_simulation(data, 1), 1);
	}
	while (i < data->num_of_philos)
	{
		if (pthread_create(&data->threads[i], NULL, (void *)routine, \
		(void *)&data->philos[i]) != 0)
			return (end_simulation(data, i - 1), destroy_all(data));
		i++;
	}
	monitor(data);
	return (end_simulation(data, data->num_of_philos), 1);
}
