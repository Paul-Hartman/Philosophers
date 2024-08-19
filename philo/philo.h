/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:43:34 by phartman          #+#    #+#             */
/*   Updated: 2024/08/19 15:08:27 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

struct	s_vars;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long long		last_meal;
	pthread_t		thread;
	struct s_vars	*vars;
}					t_philo;

typedef struct s_vars
{
	int				nr_of_forks;
	int				nr_of_philos;
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				nr_of_meals;
	int				died;
	int				all_full;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	check_meal;
	pthread_mutex_t	printing;
	t_philo			*philos;
}					t_vars;

// philo.c
int					main(int argc, char const *argv[]);
void				*philo_action(void *arg);
void				eat(t_philo *philo);
int is_full(t_vars *vars);

// utils
int					ft_atoi(const char *nptr);
void				safe_print(t_vars *vars, int id, char *string);
void				safe_sleep(long long wait_time, t_vars *vars);
long long			get_time(void);
long long			get_time_diff(long long cur_time, long long last_meal);

// init
void				init_vars(int argc, char const *argv[], t_vars *vars);
void				create_mutexes(t_vars *vars);
void				init_philos(t_vars *vars);
void				create_threads(t_vars *vars);
void				assign_forks(t_philo *philo, pthread_mutex_t **lower_fork,
						pthread_mutex_t **higher_fork);

// end
void				death_checker(t_vars *vars);
void				check_all_full(t_vars *vars);
void				destroy_mutexes(t_vars *vars);
void				join_threads(t_vars *vars);
int					is_dead(t_vars *vars);

#endif
