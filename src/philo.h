/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: eeklund <eeklund@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/13 15:51:47 by eeklund       #+#    #+#                 */
/*   Updated: 2024/10/20 16:01:25 by eeklund       ########   odam.nl         */
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

# define START_LOCK 0
# define EAT_LOCK 1
# define DEAD_LOCK 2
# define WRITE_LOCK 3

typedef struct s_philo
{
	pthread_t		p_thread;
	struct s_data	*data;
	int				num;
	int				type;
	size_t			last_meal;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	int				meals_eaten;
}	t_philo;

typedef struct s_data
{
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	int				num_of_philos;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			time_to_die;
	size_t			start_time;
	int				meals_limit;
	bool			philos_ready;
	bool			dead;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	start_lock;
	pthread_mutex_t	eat_lock;
	pthread_mutex_t	write_lock;
}	t_data;

/* input_check */
int		input_ok(int ac, char **av);
int		ft_isdigit(int c);
int		destroy_all(t_data *data);

/* init */
int		init_forks(t_data *data, int n);
void	init_philos(t_data *data, char **input);
int		init_data(t_data *data, char **input, int num);
int		init(t_data *data, char **input);

/* threads */
void	monitor(t_data *data);
void	routine(void *philo_ptr);
int		start_threads(t_data *data);

/* actions */
void	grab_forks_even(t_philo *philo, pthread_mutex_t *fir, pthread_mutex_t *sec);
void	grab_forks_odd(t_philo *philo);
void	drop_forks(t_philo *philo);
void	drop_forks_even(t_philo *philo);
void	drop_forks_odd(t_philo *philo);
int		eating(t_philo *philo);

/* utils */
int		print_message(char *str, t_philo *philo);
void	print_safe(t_philo *philo, size_t timestamp, int philo_num, char *str);
void	apply_mutx_bool(pthread_mutex_t *mutex, bool *old_var, bool new_var);
void	apply_mutx(pthread_mutex_t *mutex, size_t *old, size_t new);

/* threads_utils */
int		philo_is_dead(t_philo *philos);
int		all_philos_alive(t_philo *philo);
int		all_fed(t_data *data);

size_t	get_time(void);
int		ft_usleep(size_t millisecs);
long	ft_atoi(const char *nptr);
int		ft_strncmp(const char *s1, const char *s2);

#endif
