/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:44:45 by phartman          #+#    #+#             */
/*   Updated: 2024/07/31 18:57:05 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_action(void *arg)
{
	t_philo	*philosopher;
	struct timeval tv;

	philosopher = (t_philo *)arg.philos;
	while(1)
	{
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d is thinking", tv.tv_sec, tv.tv_usec, philosopher->id);
		pthread_mutex_lock(philosopher->left_fork);
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d has taken a fork\n", tv.tv_sec, tv.tv_usec, philosopher->id);
		pthread_mutex_lock(philosopher->right_fork);
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d has taken a fork\n", tv.tv_sec, tv.tv_usec, philosopher->id);
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d is eating", tv.tv_sec, tv.tv_usec, philosopher->id);
		usleep(philosopher->vars->time_to_eat);
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d is sleeping", tv.tv_sec, tv.tv_usec, philosopher->id);
		usleep(philosopher->vars->time_to_sleep);
		if(philosopher->last_meal < philosopher->vars->time_to_die)
		{
			gettimeofday(&tv, NULL);
			printf("◦ %ld.%06ld %d died", gettimeofday(&tv, NULL), philosopher->id);
			return ;
		}
		
	}
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

void	create_threads(t_vars vars)
{
	int	i;

	i = 0;
	while (i < vars.nr_of_philos)
	{
		pthread_create(vars.philos[i].thread, NULL, philo_action,
			&vars);
		i++;
	}
}

void	create_mutexes(t_vars vars)
{
	int	i;

	i = 0;
	while (i < vars.nr_of_forks)
		pthread_mutex_init(&vars.forks[i++], NULL);
}

void	join_threads(t_vars vars)
{
	int	i;

	i = 0;
	while (i < vars.nr_of_philos)
		pthread_join(*vars.philos[i++].thread, NULL);
}

void	destroy_mutexes(t_vars vars)
{
	int	i;

	i = 0;
	while (i < vars.nr_of_forks)
		pthread_mutex_destroy(&vars.forks[i++]);
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
	vars.time_to_die = ft_atoi(argv[2]);
	vars.time_to_eat = ft_atoi(argv[3]);
	vars.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		vars.nr_of_meals = ft_atoi(argv[5]);
	else
		vars.nr_of_meals = -1;
	create_mutexes(vars);
	create_threads(vars);
	join_threads(vars);
	destroy_mutexes(vars);
	return (0);
}
