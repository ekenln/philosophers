/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/05 19:24:14 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/26 14:13:05 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//pthread_mutex_destroy on all forks and free fork array
void	destroy_forks(t_mutex *forks, int num_of_forks)
{
	int	i;

	i = 0;
	while (i < num_of_forks)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
	forks = NULL;
}

//destroys forks and frees all allocated memory
int	destroy_all(t_data *data)
{
	if (data->forks)
		destroy_forks(data->forks, data->num_of_philos);
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
	if (data->threads)
	{
		free(data->threads);
		data->threads = NULL;
	}
	return (0);
}
