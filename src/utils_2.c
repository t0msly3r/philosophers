/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiz-ben <tfiz-ben@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:40:52 by tfiz-ben          #+#    #+#             */
/*   Updated: 2025/06/06 10:17:14 by tfiz-ben         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	ft_initialize_philos(t_philo *philosophers, t_rules *rules,
		pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < rules->number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		philosophers[i].id = i + 1;
		philosophers[i].eat_count = 0;
		philosophers[i].last_meal = get_time_in_ms();
		philosophers[i].left_fork = &forks[i];
		philosophers[i].right_fork = &forks[(i + 1)
			% rules->number_of_philosophers];
		philosophers[i].rules = rules;
		i++;
	}
}

void	ft_initialize_rules(t_rules *rules, int argc, char **argv,
		t_philo *philo)
{
	rules->number_of_philosophers = atoi(argv[1]);
	rules->time_to_die = atoi(argv[2]);
	rules->time_to_eat = atoi(argv[3]);
	rules->time_to_sleep = atoi(argv[4]);
	if (argc < 6)
		rules->must_eat = -1;
	else
		rules->must_eat = atoi(argv[5]);
	rules->simulation_stopped = 0;
	rules->start_time = get_time_in_ms();
	rules->philos = philo;
	pthread_mutex_init(&rules->print_mutex, NULL);
	pthread_mutex_init(&rules->death_mutex, NULL);
}

void	ft_create_threads(t_philo *philosophers, t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->number_of_philosophers)
	{
		if (pthread_create(&philosophers[i].thread, NULL, philosopher,
				&philosophers[i]) != 0)
		{
			perror("Failed to create philosopher thread");
			return ;
		}
		i++;
	}
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	minus;
	int	nbr;

	nbr = 0;
	i = 0;
	minus = 0;
	while ((char)nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if ((char)nptr[i] == '-')
	{
		minus = 1;
		i++;
	}
	else if ((char)nptr[i] == '+')
		i++;
	while (ft_isdigit((int)nptr[i]))
	{
		nbr *= 10;
		nbr += nptr[i] - '0';
		i++;
	}
	if (minus)
		nbr *= -1;
	return (nbr);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}
