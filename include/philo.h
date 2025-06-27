/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiz-ben <tfiz-ben@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:41:04 by tfiz-ben          #+#    #+#             */
/*   Updated: 2025/06/26 12:44:04 by tfiz-ben         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				simulation_stopped;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	struct s_philo	*philos;
}					t_rules;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules			*rules;
}					t_philo;

long long			get_time_in_ms(void);
void				print_state(t_philo *philo, char *msg);
void				stop_simulation(t_rules *rules);
int					check_simulation_stopped(t_rules *rules);
void				*monitor(void *arg);
int					check_times_eaten(t_philo *philo);
void				ft_create_threads(t_philo *philosophers, t_rules *rules);
void				ft_initialize_philos(t_philo *philosophers, t_rules *rules,
						pthread_mutex_t *forks);
void				ft_initialize_rules(t_rules *rules, int argc, char **argv,
						t_philo *philo);
void				check_arguments(int argc, char **argv);
int					manage_variables(char **argv, pthread_mutex_t **forks,
						t_philo **philosophers);
void				*philosopher(void *arg);
void				ft_manage_threads(t_philo *philosophers, t_rules *rules,
						pthread_t *monitor_thread);
void				ft_cleanup(t_rules *rules, t_philo *philosophers,
						pthread_mutex_t *forks);
int					check_times_eaten(t_philo *philo);
int					ft_atoi(const char *nptr);
int					ft_isdigit(int c);
void				ft_usleep(int ms);

#endif