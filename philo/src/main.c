/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/01 14:42:08 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/20 16:28:49 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (!input_ok(ac, av))
		return (1);
	if (!init(&data, av))
		return (1);
	if (!start_threads(&data))
		return (1);
	destroy_all(&data);
	return (0);
}
