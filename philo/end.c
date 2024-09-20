/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:23:12 by phartman          #+#    #+#             */
/*   Updated: 2024/09/20 17:43:05 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	death_checker(t_vars *vars)
{
	int	i;
	int	j;

	while (!vars->all_full)
	{
		i = -1;
		j = 0;
		while (++i < vars->nr_of_philos && !(is_dead(vars)))
		{
			pthread_mutex_lock(&vars->check_meal);
			if (get_time_diff(get_time(),
					vars->philos[i].last_meal) > vars->time_to_die)
			{
				safe_print(vars, i, "died");
				vars->died = 1;
			}
			pthread_mutex_unlock(&vars->check_meal);
		}
		if (is_dead(vars))
			break ;
		check_all_full(vars);
	}
}

static void	check_all_full(t_vars *vars)
{
	int	i;

	i = 0;
	while (vars->philos[i].meals_eaten == vars->nr_of_meals
		&& vars->nr_of_meals != -1)
		i++;
	if (i == vars->nr_of_philos)
	{
		pthread_mutex_lock(&vars->check_meal);
		vars->all_full = 1;
		pthread_mutex_unlock(&vars->check_meal);
	}
}

void	join_threads(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nr_of_philos)
	{
		if(pthread_join(vars->philos[i].thread, NULL))
		{
			printf("failed to join threads");
			exit(1);
		}
		i++;
	}
	free(vars->philos);
}

void	destroy_mutexes(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nr_of_forks)
		pthread_mutex_destroy(&vars->forks[i++]);
	pthread_mutex_destroy(&vars->check_meal);
	pthread_mutex_destroy(&vars->printing);
	free(vars->forks);
}

int	is_dead(t_vars *vars)
{
	int	i;

	pthread_mutex_lock(&vars->check_meal);
	i = vars->died;
	pthread_mutex_unlock(&vars->check_meal);
	return (i);
}
