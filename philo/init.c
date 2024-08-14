/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:16:58 by phartman          #+#    #+#             */
/*   Updated: 2024/08/14 18:22:37 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_vars(int argc, char const *argv[], t_vars *vars)
{
	vars->nr_of_philos = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	vars->nr_of_forks = vars->nr_of_philos;
	vars->died = 0;
	vars->all_full = 0;
	vars->start_time = get_time();
	if (argc == 6)
		vars->nr_of_meals = ft_atoi(argv[5]);
	else
		vars->nr_of_meals = -1;
}

void	create_mutexes(t_vars *vars)
{
	int	i;

	i = 0;
	vars->forks = malloc(vars->nr_of_forks * sizeof(pthread_mutex_t));
	while (i < vars->nr_of_forks)
		pthread_mutex_init(&vars->forks[i++], NULL);
	pthread_mutex_init(&vars->check_meal, NULL);
	pthread_mutex_init(&vars->printing, NULL);
}

void	init_philos(t_vars *vars)
{
	int	i;

	i = 0;
	vars->philos = malloc(vars->nr_of_philos * sizeof(t_philo));
	if (vars->philos == NULL)
	{
		printf("malloc failed");
		exit(1);
	}
	while (i < vars->nr_of_philos)
	{
		vars->philos[i].id = i;
		vars->philos[i].vars = vars;
		vars->philos[i].meals_eaten = 0;
		vars->philos[i].left_fork = &vars->forks[i];
		vars->philos[i].right_fork = &vars->forks[(i + 1) % vars->nr_of_philos];
		vars->philos[i].last_meal = get_time();
		i++;
	}
}

void	create_threads(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nr_of_philos)
	{
		if (pthread_create(&vars->philos[i].thread, NULL, philo_action,
				&vars->philos[i]) != 0)
		{
			printf("failed to create threads");
			exit(1);
		}
		i++;
	}
}

void	assign_forks(t_philo *philo, pthread_mutex_t **lower_fork,
		pthread_mutex_t **higher_fork)
{
	if (philo->left_fork < philo->right_fork)
	{
		*lower_fork = philo->left_fork;
		*higher_fork = philo->right_fork;
	}
	else
	{
		*lower_fork = philo->right_fork;
		*higher_fork = philo->left_fork;
	}
}
