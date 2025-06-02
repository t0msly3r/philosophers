#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include "../lib/libft.h"

typedef struct s_rules {
	int number_of_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int must_eat;
	int simulation_stopped;
	long start_time;
	pthread_mutex_t *forks;
	pthread_mutex_t print_mutex;
	pthread_mutex_t death_mutex;
	struct s_philo *philos;
} t_rules;

typedef struct s_philo {
	int id;
	int eat_count;
	long last_meal;
	pthread_t thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	t_rules *rules;
} t_philo;

long long	get_time_in_ms(void);
void 		print_state(t_philo *philo, char *msg);
void stop_simulation(t_rules *rules);
int check_simulation_stopped(t_rules *rules);
void *monitor(void *arg);
int check_times_eaten(t_philo *philo);

#endif