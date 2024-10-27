/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/13 15:51:47 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/27 17:38:17 by eeklund       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <stdbool.h>

# define TAKE_FORKS "has taken a fork"
# define THINKING "is thinking"
# define SLEEPING "is sleeping"
# define EATING "is eating"
# define DIED "\033[31;1mdied\033[0m"

typedef pthread_mutex_t	t_mutex;

typedef struct s_philo
{
	pthread_t		p_thread;
	struct s_data	*data;
	int				num;
	int				type;
	size_t			last_meal;
	t_mutex			*fork_left;
	t_mutex			*fork_right;
	int				meals_eaten;
}	t_philo;

typedef struct s_data
{
	pthread_t		*threads;
	t_mutex			*forks;
	t_philo			*philos;
	int				num_of_philos;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			time_to_die;
	size_t			start_time;
	int				meals_limit;
	bool			philos_ready;
	bool			dead;
	t_mutex			dead_lock;
	t_mutex			start_lock;
	t_mutex			eat_lock;
	t_mutex			write_lock;
}	t_data;

/* input_check */
int		input_ok(int ac, char **av);
long	ft_atoi(const char *nptr);

/* init */
int		init(t_data *data, char **input);

/* threads */
int		start_threads(t_data *data);

/* actions */
int		eating(t_philo *philo);

/* utils */
int		print_message(char *str, t_philo *philo);
void	print_safe(t_philo *philo, size_t timestamp, int philo_num, char *str);
void	mutx_bool(t_mutex *mutex, bool *old, bool new);
size_t	get_time(void);
int		ft_usleep(size_t millisecs);

/* threads_utils */
int		philo_is_dead(t_philo *philos);
int		all_philos_alive(t_philo *philo);
int		all_fed(t_data *data);
void	handle_one_philo(void *philo_ptr);
void	end_simulation(t_data *data, int threads_created);

/* error */
int		destroy_all(t_data *data);
void	destroy_forks(t_mutex *forks, int num_of_forks);

#endif
