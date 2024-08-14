/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:44:45 by phartman          #+#    #+#             */
/*   Updated: 2024/08/14 18:36:04 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	t_vars			*vars;
	pthread_mutex_t	*lower_fork;
	pthread_mutex_t	*higher_fork;

	assign_forks(philo, &lower_fork, &higher_fork);
	vars = philo->vars;
	pthread_mutex_lock(lower_fork);
	safe_print(vars, philo->id, "has taken a fork");
	if (vars->nr_of_philos == 1)
	{
		safe_sleep(vars->time_to_die, vars);
		pthread_mutex_unlock(lower_fork);
		return ;
	}
	pthread_mutex_lock(higher_fork);
	safe_print(vars, philo->id, "has taken a fork");
	pthread_mutex_lock(&vars->check_meal);
	safe_print(vars, philo->id, "is eating");
	philo->last_meal = get_time();
	pthread_mutex_unlock(&vars->check_meal);
	safe_sleep(vars->time_to_eat, vars);
	philo->meals_eaten++;
	pthread_mutex_unlock(higher_fork);
	pthread_mutex_unlock(lower_fork);
}

void	*philo_action(void *arg)
{
	t_vars	*vars;
	t_philo	*philo;

	philo = (t_philo *)arg;
	vars = philo->vars;
	if (philo->id % 2 == 0)
		safe_sleep(1, vars);
	while (!is_dead(vars) && (!vars->all_full || vars->nr_of_meals == -1))
	{
		eat(philo);
		if (vars->all_full)
			break ;
		safe_print(vars, philo->id, "is sleeping");
		safe_sleep(vars->time_to_sleep, vars);
		safe_print(vars, philo->id, "is thinking");
	}
	return (NULL);
}

int	main(int argc, char const *argv[])
{
	t_vars	vars;

	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	init_vars(argc, argv, &vars);
	if (vars.nr_of_philos < 1 || vars.nr_of_philos > 200
		|| vars.time_to_die < 60 || vars.time_to_eat < 60
		|| vars.time_to_sleep < 60)
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	create_mutexes(&vars);
	init_philos(&vars);
	create_threads(&vars);
	death_checker(&vars);
	join_threads(&vars);
	destroy_mutexes(&vars);
	return (0);
}
