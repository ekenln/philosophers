/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 16:43:53 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/20 16:14:56 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	grab_forks(t_philo *philo, pthread_mutex_t *fir, pthread_mutex_t *sec)
{
	pthread_mutex_lock(fir);
	print_message(TAKE_FORKS, philo);
	pthread_mutex_lock(sec);
	print_message(TAKE_FORKS, philo);
}

void	drop_forks_even(t_philo *philo)
{
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
}

void	drop_forks_odd(t_philo *philo)
{
	pthread_mutex_unlock(philo->fork_right);
	pthread_mutex_unlock(philo->fork_left);
}

void	drop_forks(t_philo *philo)
{
	if (philo->type == 0)
		drop_forks_even(philo);
	else
		drop_forks_odd(philo);
}

int	eating(t_philo *philo)
{
	bool	dead;

	dead = false;
	apply_mutx_bool(&philo->data->dead_lock, \
	&dead, philo->data->dead);
	if (dead == true)
	{
		drop_forks(philo);
		return (0);
	}
	if (philo->type == 0)
		grab_forks(philo, philo->fork_left, philo->fork_right);
	else
		grab_forks(philo, philo->fork_right, philo->fork_left);
	apply_mutx(&philo->data->eat_lock, &philo->last_meal, get_time());
	apply_mutx_bool(&philo->data->dead_lock, \
	&dead, philo->data->dead);
	if (dead == true)
	{
		drop_forks(philo);
		return (0);
	}
	print_message(EATING, philo);
	pthread_mutex_lock(&philo->data->eat_lock);
	philo->meals_eaten += 1;
	pthread_mutex_unlock(&philo->data->eat_lock);
	ft_usleep(philo->data->time_to_eat);
	drop_forks(philo);
	return (1);
}
