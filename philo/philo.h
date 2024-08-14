/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:43:34 by phartman          #+#    #+#             */
/*   Updated: 2024/08/14 13:59:33 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H


# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>
#include <stddef.h>


struct s_vars;

typedef struct s_philo
{
	int id;
	int meals_eaten;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	long long last_meal;
	pthread_t thread;
	struct s_vars *vars;
}				t_philo;

typedef struct s_vars
{
	int nr_of_forks;
	int nr_of_philos;
	int time_to_eat;
	int time_to_die;
	int time_to_sleep;
	int nr_of_meals;
	int died;
	int all_full;
	pthread_mutex_t *forks;
	pthread_mutex_t check_meal;
	pthread_mutex_t printing;
	t_philo *philos;
}				t_vars;



# endif

