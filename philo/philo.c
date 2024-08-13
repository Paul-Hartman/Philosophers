/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:44:45 by phartman          #+#    #+#             */
/*   Updated: 2024/08/13 21:02:45 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


long long get_time_diff(long long cur_time, long long last_meal)
{
	long long diff;

	diff = cur_time- last_meal;
	return (diff);
}

long long get_time(void)
{
	struct timeval t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec/1000));

}

void safe_print(t_vars *vars, int id, char *string)
{
	pthread_mutex_lock(&vars->printing);
	if(!vars->died)
		printf("%lli %i %s\n", get_time(), id, string);
	pthread_mutex_unlock(&vars->printing);
}

void safe_sleep(long long wait_time, t_vars *vars)
{
	long long i;

	i = get_time();
	while(!(vars->died) && get_time_diff(i, get_time()) < wait_time)
		usleep(50);
}


void death_checker(t_vars *vars)
{
	unsigned int i;
	unsigned int j;

	while(!vars->all_full)
	{
		i = 0;
		j = 0;
		while (i < vars->nr_of_philos && !(vars->died))
		{
			pthread_mutex_lock(&vars->check_meal);
			if (get_time_diff(get_time(), vars->philos[i].last_meal) > vars->time_to_die)
			{
				safe_print(vars, i, "has died");
				vars->died = 1;
			}
			pthread_mutex_unlock(&vars->check_meal);
			usleep(100);
			if (vars->philos[i].meals_eaten == vars->nr_of_meals && vars->nr_of_meals != -1)
				j++;
			
			i++;
		}
		if (vars->died)
			break ;
		if (j == vars->nr_of_philos)
			vars->all_full = 1;
	}
}

void eat(t_philo *philo)
{
		int left;
		int right;
		t_vars *vars;

		vars = philo->vars; 
		left = philo->id;
		right = (philo->id + 1) % vars->nr_of_forks;
		int lower_fork = left < right ? left : right;
		int higher_fork = left < right ? right : left;
			//take forks
		
		pthread_mutex_lock(&vars->forks[lower_fork]);
		safe_print(vars, philo->id, "has taken a fork");
		pthread_mutex_lock(&vars->forks[higher_fork]);
		safe_print(vars, philo->id, "has taken a fork");
		
		//eat
		safe_print(vars, philo->id, "is eating");
		philo->last_meal = get_time();
		safe_sleep(vars->time_to_eat, vars);
		philo->meals_eaten++;

		//release forks
		pthread_mutex_unlock(&vars->forks[higher_fork]);
		pthread_mutex_unlock(&vars->forks[lower_fork]);
}

void	*philo_action(void *arg)
{
	t_vars	*vars;
	t_philo *philo; 
	philo = (t_philo *)arg;
	vars = philo->vars;

	while(!vars->died)
	{
		eat(philo);
		if(vars->all_full)
			break;
		//sleep
		safe_print(vars, philo->id, "is sleeping");
		safe_sleep(vars->time_to_sleep, vars);
		//think
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
	unsigned int	i;

	i = 0;
	vars->philos = malloc(vars->nr_of_philos * sizeof(t_philo));
	if(vars->philos == NULL)
	{
		printf("malloc failed");
		exit(1);
	}
		
	while (i < vars->nr_of_philos)
	{
		vars->philos[i].id = i;
		vars->philos[i].vars = vars;
		vars->philos[i].meals_eaten = 0;
		vars->philos[i].left_fork_id = i;
		vars->philos[i].right_fork_id = (i + 1) % vars->nr_of_philos;
		vars->philos[i].last_meal = get_time();
		if(pthread_create(&vars->philos[i].thread, NULL, philo_action,
			&vars->philos[i]) != 0)
		{
			printf("failed to create threads");
			exit(1);
		}
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
	pthread_mutex_init(&vars->check_meal, NULL);
	pthread_mutex_init(&vars->printing, NULL);
}

void	join_threads(t_vars *vars)
{
	unsigned int	i;

	i = 0;
	while (i < vars->nr_of_philos)
	{
		 printf("Joining thread %u\n", i);
		 pthread_join(vars->philos[i++].thread, NULL);
		 printf("Joined thread %u\n", i);
	}
		
	free(vars->philos);
}

void	destroy_mutexes(t_vars *vars)
{
	unsigned int	i;

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
	vars.died = 0;
	vars.all_full = 0;
	if(vars.nr_of_philos < 1 || vars.nr_of_philos > 200)

	if (argc == 6)
		vars.nr_of_meals = ft_atoi(argv[5]);
	else
		vars.nr_of_meals = -1;
		
	create_mutexes(&vars);
	create_threads(&vars);
	death_checker(&vars);
	destroy_mutexes(&vars);
	join_threads(&vars);
	
	return (0);
}
