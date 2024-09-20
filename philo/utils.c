/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phartman <phartman@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 18:28:36 by phartman          #+#    #+#             */
/*   Updated: 2024/09/20 17:21:33 by phartman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

long long	get_time_diff(long long cur_time, long long last_meal)
{
	long long	diff;

	diff = cur_time - last_meal;
	return (diff);
}

long long	get_time(void)
{
	struct timeval	t;
	long long		default_time;

	default_time = 0;
	if (gettimeofday(&t, NULL) == -1)
	{
		printf("gettimeofday failed\n");
		return (default_time);
	}
	return ((t.tv_sec * 1000 + t.tv_usec / 1000));
}

void	safe_sleep(long long wait_time, t_vars *vars)
{
	long long	start_time;
	long long	current_time;

	start_time = get_time();
	while (!is_dead(vars))
	{
		current_time = get_time();
		if (get_time_diff(current_time, start_time) >= wait_time)
			break ;
		usleep(50);
	}
}

void	safe_print(t_vars *vars, int id, char *string)
{
	long long	current_time;
	long long	elapsed_time;

	pthread_mutex_lock(&vars->printing);
	if (!vars->died)
	{
		current_time = get_time();
		elapsed_time = current_time - vars->start_time;
		printf("%lli %i %s\n", elapsed_time, id, string);
	}
	pthread_mutex_unlock(&vars->printing);
}
