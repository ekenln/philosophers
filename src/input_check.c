/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_check.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/13 18:09:33 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/20 12:25:16 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	input_ok(int ac, char **av)
{
	int		i;
	int		j;

	if (ac != 5 && ac !=6)
		return (0);
	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
			{
				printf("Wrong input, only numeric chars are allowed\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

void	destroy_forks(pthread_mutex_t *forks, int num_of_forks)
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
