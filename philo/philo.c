/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:44:45 by phartman          #+#    #+#             */
/*   Updated: 2024/08/12 18:12:11 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_action(void *arg)
{
	t_vars	*vars;
	int id;
	t_philo *philo; 
	philo = (t_philo *)arg;
	vars = philo->vars;
	id = philo->id;
	int left;
	int right;
	id = ((t_philo *)arg)->id;

	left = id;
	right = (id + 1) % vars->nr_of_forks;
	int lower_fork = left < right ? left : right;
int higher_fork = left < right ? right : left;
	
	//id = //get frm arg somehow
	//t_philo	*philosopher;
	struct timeval tv;

	//philosopher = (t_philo *)arg.philos;
	while(1)
	{
		printf("got to beggining of loop\n");
		//think
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d is thinking\n", tv.tv_sec, tv.tv_usec, id);
		usleep(vars->time_to_sleep * 1000);
		//take forks
		
		pthread_mutex_lock(&vars->forks[lower_fork]);
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d has taken a fork\n", tv.tv_sec, tv.tv_usec, id);
		pthread_mutex_lock(&vars->forks[higher_fork]);
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d has taken a fork\n", tv.tv_sec, tv.tv_usec, id);
	
		//eat
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d is eating\n", tv.tv_sec, tv.tv_usec, id);
		usleep(vars->time_to_eat * 1000);
		
		//release forks
		pthread_mutex_unlock(&vars->forks[higher_fork]);
		pthread_mutex_unlock(&vars->forks[lower_fork]);

		//sleep
		gettimeofday(&tv, NULL);
		printf("◦ %ld.%06ld %d is sleeping/n", tv.tv_sec, tv.tv_usec, id);
		usleep(vars->time_to_sleep * 1000);

		//check death
		// if(philosopher->last_meal < philosopher->vars->time_to_die)
		// {
		// 	gettimeofday(&tv, NULL);
		// 	printf("◦ %ld.%06ld %d died", gettimeofday(&tv, NULL), id);
		// 	return ;
		// }
		printf("got to end of loop\n");
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

void	create_threads(t_vars *vars)
{
	unsigned int	i;
	t_philo			*philo;

	i = 0;
	vars->philos = malloc(vars->nr_of_philos * sizeof(pthread_t));
	philo = malloc(vars->nr_of_philos * sizeof(t_philo));
	while (i < vars->nr_of_philos)
	{
		philo[i].id = i;
		philo[i].vars = vars;
		pthread_create(&vars->philos[i], NULL, philo_action,
			&philo[i]);
		i++;
	}
}

void	create_mutexes(t_vars *vars)
{
	unsigned int	i;

	i = 0;
	vars->forks = malloc(vars->nr_of_forks * sizeof(pthread_mutex_t));
	while (i < vars->nr_of_forks)
		pthread_mutex_init(&vars->forks[i++], NULL);
}

void	join_threads(t_vars *vars)
{
	unsigned int	i;

	i = 0;
	while (i < vars->nr_of_philos)
		pthread_join(vars->philos[i++], NULL);
}

void	destroy_mutexes(t_vars *vars)
{
	unsigned int	i;

	i = 0;
	while (i < vars->nr_of_forks)
		pthread_mutex_destroy(&vars->forks[i++]);
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
	if (argc == 6)
		vars.nr_of_meals = ft_atoi(argv[5]);
	else
		vars.nr_of_meals = -1;
		
	create_mutexes(&vars);
	create_threads(&vars);
	join_threads(&vars);
	destroy_mutexes(&vars);
	return (0);
}
