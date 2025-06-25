/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiz-ben <tfiz-ben@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:40:55 by tfiz-ben          #+#    #+#             */
/*   Updated: 2025/06/24 11:35:04 by tfiz-ben         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	check_simulation_stopped(t_rules *rules)
{
	int	stopped;

	pthread_mutex_lock(&rules->death_mutex);
	stopped = rules->simulation_stopped;
	pthread_mutex_unlock(&rules->death_mutex);
	return (stopped);
}

void	stop_simulation(t_rules *rules)
{
	pthread_mutex_lock(&rules->death_mutex);
	rules->simulation_stopped = 1;
	pthread_mutex_unlock(&rules->death_mutex);
}

int	manage_variables(char **argv, pthread_mutex_t **forks,
		t_philo **philosophers)
{
	int	n;

	n = ft_atoi(argv[1]);
	if (n <= 0 || n > 200)
	{
		printf("Wrong number of philosophers.\n");
		return (0);
	}
	*forks = malloc(n * sizeof(pthread_mutex_t));
	if (!(*forks))
	{
		perror("Failed to allocate memory for forks");
		return (0);
	}
	*philosophers = malloc(n * sizeof(t_philo));
	if (!(*philosophers))
	{
		perror("Failed to allocate memory for philosophers");
		free(forks);
		return (0);
	}
	return (1);
}

void	ft_manage_threads(t_philo *philosophers, t_rules *rules,
		pthread_t *monitor_thread)
{
	int	i;

	i = 0;
	ft_create_threads(philosophers, rules);
	if (pthread_create(monitor_thread, NULL, monitor, (void *)rules) != 0)
	{
		perror("Failed to create monitor thread");
		return ;
	}
	while (i < rules->number_of_philosophers)
	{
		pthread_join(philosophers[i].thread, NULL);
		i++;
	}
	pthread_join(*monitor_thread, NULL);
}

int	main(int argc, char **argv)
{
	t_rules			rules;
	pthread_mutex_t	*forks;
	pthread_t		monitor_thread;
	t_philo			*philosophers;

	check_arguments(argc, argv);
	if (!manage_variables(argv, &forks, &philosophers))
		return (1);
	ft_initialize_rules(&rules, argc, argv, philosophers);
	rules.forks = forks;
	ft_initialize_philos(philosophers, &rules, forks);
	ft_manage_threads(philosophers, &rules, &monitor_thread);
	ft_cleanup(&rules, philosophers, forks);
}
