/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:44:45 by phartman          #+#    #+#             */
/*   Updated: 2024/08/14 16:54:53 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time_diff(long long cur_time, long long last_meal)
{
	long long	diff;

	diff = cur_time - last_meal;
	return (diff);
}

long long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000 + t.tv_usec / 1000));
}

void	safe_sleep(long long wait_time, t_vars *vars)
{
	long long	start_time;
	long long	current_time;

	start_time = get_time();
	while (!(vars->died))
	{
		current_time = get_time();

		if (get_time_diff(current_time, start_time) >= wait_time)
			break ;
		usleep(50);
	}
}

void	safe_print(t_vars *vars, int id, char *string)
{
	pthread_mutex_lock(&vars->printing);
	if (!vars->died)
	{
		long long current_time = get_time();
        long long elapsed_time = current_time - vars->start_time;
		printf("%lli %i %s\n", elapsed_time, id, string);
	}
	pthread_mutex_unlock(&vars->printing);
}

void	death_checker(t_vars *vars)
{
	int	i;
	int	j;

	while (!vars->all_full)
	{
		i = 0;
		j = 0;
		while (i < vars->nr_of_philos && !(vars->died))
		{
			pthread_mutex_lock(&vars->check_meal);
			if (get_time_diff(get_time(),
					vars->philos[i].last_meal) > vars->time_to_die)
			{
				safe_print(vars, i, "died");
				vars->died = 1;
			}
			pthread_mutex_unlock(&vars->check_meal);
			if (vars->philos[i].meals_eaten == vars->nr_of_meals
				&& vars->nr_of_meals != -1)
				j++;
			i++;
		}
		//pthread_mutex_lock(&vars->check_meal);
		if (vars->died)
		{
			//pthread_mutex_unlock(&vars->check_meal);
			break ;
		}
		//pthread_mutex_unlock(&vars->check_meal);
		if (j == vars->nr_of_philos)
			vars->all_full = 1;
	}
}

void	eat(t_philo *philo)
{
	t_vars			*vars;
	pthread_mutex_t	*lower_fork;
	pthread_mutex_t	*higher_fork;

	vars = philo->vars;
	if (philo->left_fork < philo->right_fork)
	{
		lower_fork = philo->left_fork;
		higher_fork = philo->right_fork;
	}
	else
	{
		lower_fork = philo->right_fork;
		higher_fork = philo->left_fork;
	}
	// if (philo->id % 2 == 0)
	// usleep(100);
	// take forks
	pthread_mutex_lock(lower_fork);
	safe_print(vars, philo->id, "has taken a fork");
	// if (vars->nr_of_philos == 1)
	// {
	// 	safe_sleep(vars->time_to_die, vars);
	// 	pthread_mutex_unlock(lower_fork);
	// 	return ;
	// }
	pthread_mutex_lock(higher_fork);
	safe_print(vars, philo->id, "has taken a fork");
	// eat
	pthread_mutex_lock(&vars->check_meal);
	safe_print(vars, philo->id, "is eating");
	philo->last_meal = get_time();
	pthread_mutex_unlock(&vars->check_meal);
	safe_sleep(vars->time_to_eat, vars);
	philo->meals_eaten++;
	// release forks
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
	while (!vars->died && (!vars->all_full || vars->nr_of_meals == -1))
	{
		eat(philo);
		if (vars->all_full)
			break ;
		// sleep
		safe_print(vars, philo->id, "is sleeping");
		safe_sleep(vars->time_to_sleep, vars);
		// think
		safe_print(vars, philo->id, "is thinking");
	}
	return (NULL);
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		sign;
	int		result;

	i = 0;
	sign = 1;
	result = 0;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (nptr[i])
	{
		if ((nptr[i] >= '0' && nptr[i] <= '9'))
			result = result * 10 + (nptr[i] - '0');
		else
			return (result * sign);
		i++;
	}
	return (result * sign);
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

void	join_threads(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->nr_of_philos)
	{
		pthread_join(vars->philos[i].thread, NULL);
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

int	main(int argc, char const *argv[])
{
	t_vars	vars;

	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	vars.nr_of_philos = ft_atoi(argv[1]);
	vars.nr_of_forks = vars.nr_of_philos;
	vars.time_to_die = ft_atoi(argv[2]);
	vars.time_to_eat = ft_atoi(argv[3]);
	vars.time_to_sleep = ft_atoi(argv[4]);
	vars.start_time = get_time();
	vars.died = 0;
	vars.all_full = 0;
	if (vars.nr_of_philos < 1 || vars.nr_of_philos > 200 || vars.time_to_die < 60
		|| vars.time_to_eat < 60 || vars.time_to_sleep < 60)
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	if (argc == 6)
		vars.nr_of_meals = ft_atoi(argv[5]);
	else
		vars.nr_of_meals = -1;
	create_mutexes(&vars);
	init_philos(&vars);
	create_threads(&vars);
	death_checker(&vars);
	join_threads(&vars);
	destroy_mutexes(&vars);
	return (0);
}
