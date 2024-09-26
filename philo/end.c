/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:23:12 by phartman          #+#    #+#             */
/*   Updated: 2024/09/26 17:32:43 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_all_full(t_vars *vars);

void	death_checker(t_vars *vars)
{
	int	i;

	while (!check_all_full(vars))
	{
		i = -1;
		while (++i < vars->nr_of_philos && !(is_dead(vars)))
		{
			if (vars->philos[i].meals_eaten == vars->nr_of_meals)
				continue ;
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
	}
}

static int	check_all_full(t_vars *vars)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&vars->check_meal);
	while (vars->philos[i].meals_eaten == vars->nr_of_meals
		&& vars->nr_of_meals != -1)
		i++;
	pthread_mutex_unlock(&vars->check_meal);
	if (i == vars->nr_of_philos)
		return (1);
	return (0);
}

void	join_threads(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nr_of_philos)
	{
		if (pthread_join(vars->philos[i].thread, NULL))
			printf("failed to join thread %d", vars->philos[i].id);
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
	free(vars->forks);
	pthread_mutex_destroy(&vars->check_meal);
	pthread_mutex_destroy(&vars->printing);
}

int	is_dead(t_vars *vars)
{
	int	i;

	pthread_mutex_lock(&vars->check_meal);
	i = vars->died;
	pthread_mutex_unlock(&vars->check_meal);
	return (i);
}
