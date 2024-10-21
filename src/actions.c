/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 16:43:53 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/21 11:14:57 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//4 200 205 200
//105 800 200 200
//200 800 200 200

int	grab_forks(t_philo *philo, pthread_mutex_t *fir, pthread_mutex_t *sec)
{
	pthread_mutex_lock(fir);
	print_message(TAKE_FORKS, philo);
	// if (philo->data->num_of_philos == 1)
	// {
	// 	ft_usleep(philo->data->time_to_die);
	// 	pthread_mutex_unlock(fir);
	// 	printf("HELLOOOO\n");
	// 	return (0);
	// }
	pthread_mutex_lock(sec);
	print_message(TAKE_FORKS, philo);
	return (1);
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
	int	res;

	if (!all_philos_alive(philo))
		return (0);
	if (philo->type == 0)
		res = grab_forks(philo, philo->fork_left, philo->fork_right);
	else
	{
		ft_usleep(philo->data->time_to_eat / 2);
		res = grab_forks(philo, philo->fork_right, philo->fork_left);
	}
	if (!res)
		return (0);
	if (!all_philos_alive(philo))
	{
		drop_forks(philo);
		return (0);
	}
	pthread_mutex_lock(&philo->data->eat_lock);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->eat_lock);
	print_message(EATING, philo);
	ft_usleep(philo->data->time_to_eat);
	drop_forks(philo);
	return (1);
}
