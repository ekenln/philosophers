/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 16:43:53 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/27 17:28:40 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	grab_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_left);
	print_message(TAKE_FORKS, philo);
	pthread_mutex_lock(philo->fork_right);
	print_message(TAKE_FORKS, philo);
	return (1);
}

static void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->fork_right);
	pthread_mutex_unlock(philo->fork_left);
}

int	eating(t_philo *philo)
{
	grab_forks(philo);
	pthread_mutex_lock(&philo->data->eat_lock);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->eat_lock);
	print_message(EATING, philo);
	ft_usleep(philo->data->time_to_eat);
	drop_forks(philo);
	return (1);
}
