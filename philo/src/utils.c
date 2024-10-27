/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 16:11:55 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/27 17:55:56 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	mutx_bool(t_mutex *mutex, bool *old, bool new)
{
	pthread_mutex_lock(mutex);
	*old = new;
	pthread_mutex_unlock(mutex);
}

void	print_safe(t_philo *philo, size_t timestamp, int philo_num, char *str)
{
	pthread_mutex_lock(&philo->data->write_lock);
	printf("%lu %i %s\n", timestamp, philo_num, str);
	pthread_mutex_unlock(&philo->data->write_lock);
}

/*prints message and double checks if someone died --> not write*/
int	print_message(char *str, t_philo *philo)
{
	size_t	timestamp;

	timestamp = get_time() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->dead_lock);
	if (philo->data->dead == false)
	{
		print_safe(philo, timestamp, philo->num, str);
		return (pthread_mutex_unlock(&philo->data->dead_lock), 1);
	}
	return (pthread_mutex_unlock(&philo->data->dead_lock), 0);
}

//get time in ms
size_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

//split up part of the sleep in intervals for accuracy
int	ft_usleep(size_t millisecs)
{
	size_t	half;
	size_t	target_time;
	size_t	start;

	start = get_time();
	target_time = get_time() + millisecs;
	half = millisecs / 2;
	usleep(half);
	while (get_time() - start < millisecs)
	{
		usleep(500);
	}
	return (0);
}
