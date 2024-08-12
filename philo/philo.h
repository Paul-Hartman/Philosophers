/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:43:34 by phartman          #+#    #+#             */
/*   Updated: 2024/08/12 19:51:50 by phartman         ###   ########.fr       */
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

typedef struct t_vars
{
	unsigned int nr_of_philos;
	unsigned int nr_of_forks;
	unsigned int time_to_die;
	unsigned int time_to_eat;
	unsigned int time_to_sleep;
	int nr_of_meals;
	int died;
	pthread_mutex_t *forks;
	pthread_t *philos;
}				t_vars;

// typedef struct t_philo
// {
// 	pthread_t *thread;
// 	unsigned int id;
// 	unsigned int nr_of_meals;
// 	unsigned int last_meal;
// 	pthread_mutex_t *left_fork;
// 	pthread_mutex_t *right_fork;
// 	pthread_mutex_t *print_mutex;
// 	t_vars *vars;
// }				t_philo;
typedef struct t_philo
{
	int id;
	int meals_eaten;
	struct timeval last_meal;
	t_vars *vars;
}				t_philo;

# endif