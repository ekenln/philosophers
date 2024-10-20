/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 16:11:55 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/20 16:01:11 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	apply_mutx_bool(pthread_mutex_t *mutex, bool *old_var, bool new_var)
{
	pthread_mutex_lock(mutex);
	*old_var = new_var;
	pthread_mutex_unlock(mutex);
}

void	apply_mutx(pthread_mutex_t *mutex, size_t *old, size_t new)
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

int	print_message(char *str, t_philo *philo)
{
	size_t	timestamp;
	bool	anyone_dead;

	timestamp = get_time() - philo->data->start_time;
	apply_mutx_bool(&philo->data->dead_lock, \
	&anyone_dead, philo->data->dead);
	if (!ft_strncmp(DIED, str))
	{
		// print_safe(philo, timestamp, philo->num, str);
		printf("%lu %i %s, last meal: %lu, current time: %lu\n", \
		timestamp, philo->num, str, philo->last_meal - philo->data->start_time,\
		get_time() - philo->data->start_time);
	}
	else
	{
		if (anyone_dead == false)
			print_safe(philo, timestamp, philo->num, str);
	}
	return (1);
}

size_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
	{
		//errorrrrr
		return (-1);
	}
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_usleep(size_t millisecs)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < millisecs)
		usleep(500);
	return (0);
}

long	ft_atoi(const char *nptr)
{
	int		i;
	long	res;
	int		sign;

	i = 0;
	res = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + nptr[i] - '0';
		i++;
	}
	return (res * sign);
}

int	ft_strncmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
