/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiz-ben <tfiz-ben@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:40:57 by tfiz-ben          #+#    #+#             */
/*   Updated: 2025/06/26 13:09:29 by tfiz-ben         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	*check_end_died(t_rules *rules, long current_time, long last_meal,
		int i)
{
	if (rules->must_eat != -1 && check_times_eaten(rules->philos))
	{
		print_state(&rules->philos[i], "end");
		stop_simulation(rules);
		return (NULL);
	}
	else if (current_time - last_meal > rules->time_to_die)
	{
		print_state(&rules->philos[i], "died");
		stop_simulation(rules);
		return (NULL);
	}
	return (" ");
}

void	*monitor(void *arg)
{
	t_rules	*rules;
	int		i;
	long	current_time;
	long	last_meal;

	rules = (t_rules *)arg;
	while (!check_simulation_stopped(rules))
	{
		i = 0;
		while (i < rules->number_of_philosophers)
		{
			current_time = get_time_in_ms();
			last_meal = rules->philos[i].last_meal;
			if (!check_end_died(rules, current_time, last_meal, i))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

static void	take_fork(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "has taken a fork");
	}
}

static void	left_fork(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	print_state(philo, "is sleeping");
}

void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->rules->number_of_philosophers == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "has taken a fork");
		ft_usleep(philo->rules->time_to_die);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(500);
	while (!check_simulation_stopped(philo->rules) && !check_times_eaten(philo))
	{
		take_fork(philo);
		print_state(philo, "is eating");
		philo->eat_count++;
		philo->last_meal = get_time_in_ms();
		ft_usleep(philo->rules->time_to_eat);
		left_fork(philo);
		ft_usleep(philo->rules->time_to_sleep);
		print_state(philo, "is thinking");
	}
	return (NULL);
}
