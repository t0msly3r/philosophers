#include "../include/philo.h"

void    *philosopher(void *arg)
{
    t_philo	*philo;

	philo = (t_philo *)arg;
	while (!check_simulation_stopped(philo->rules) || check_times_eaten(philo))
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
		pthread_mutex_lock(philo->right_fork);
		}
		// Simulate eating
        print_state(philo, "is eating");
        philo->eat_count++;
        philo->last_meal = get_time_in_ms(); // Replace with actual time logic

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
		// Simulate thinking or sleeping
        usleep(philo->rules->time_to_eat * 1000); // Simulate eating time
        print_state(philo, "is sleeping");
        usleep(philo->rules->time_to_sleep * 1000); // Simulate sleeping time
        print_state(philo, "is thinking");
	}
	return (NULL);
}

void    ft_create_threads(t_philo *philosophers, t_rules *rules)
{
    int i;

    i = 0;
    while (i < rules->number_of_philosophers)
    {
        if (pthread_create(&philosophers[i].thread, NULL, philosopher, &philosophers[i]) != 0)
        {
            perror("Failed to create philosopher thread");
            return;
        }
        i++;
    }
}
void    ft_initialize_philos(t_philo *philosophers, t_rules *rules, pthread_mutex_t *forks)
{
    int             i;

    i = 0;
    while (i < rules->number_of_philosophers)
    {
        pthread_mutex_init(&forks[i], NULL);
        philosophers[i].id = i + 1;
        philosophers[i].eat_count = 0;
        philosophers[i].last_meal = 0; // Initialize with actual time logic
        philosophers[i].left_fork = &forks[i];
        philosophers[i].right_fork = &forks[(i + 1) % rules->number_of_philosophers];
        philosophers[i].rules = rules;
        i++;
    }
}

void    ft_initialize_rules(t_rules *rules, int argc, char **argv)
{
    rules->number_of_philosophers = atoi(argv[1]);
    rules->time_to_die = atoi(argv[2]);
    rules->time_to_eat = atoi(argv[3]);
    rules->time_to_sleep = atoi(argv[4]);
    if (argc < 6)
        rules->must_eat = -1; // Default value if not provided
    else
        rules->must_eat = atoi(argv[5]);
    rules->simulation_stopped = 0;
    rules->start_time = 0; // Initialize with actual start time logic
    pthread_mutex_init(&rules->print_mutex, NULL);
    pthread_mutex_init(&rules->death_mutex, NULL);
}

int main(int argc, char **argv)
{
    if (argc < 5 || argc > 6)
    {
        fprintf(stderr, "Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [must_eat]\n", argv[0]);
        return 1;
    }
    t_rules         rules;
    pthread_mutex_t *forks;
    pthread_t        monitor_thread;
    t_philo         *philosophers;
    int             n;

    n = ft_atoi(argv[1]);
    forks = malloc(n * sizeof(pthread_mutex_t));
    if (!forks)
    {
        perror("Failed to allocate memory for forks");
        return 1;
    }
    philosophers = malloc(n * sizeof(t_philo));
    if (!philosophers)
    {
        perror("Failed to allocate memory for philosophers");
        free(forks);
        return 1;
    }
    ft_initialize_rules(&rules, argc, argv);
    rules.forks = forks;
    ft_initialize_philos(philosophers, &rules, forks);

    // Start philosopher threads
    ft_create_threads(philosophers, &rules);
    pthread_create(&monitor_thread, NULL, monitor, philosophers);
    pthread_join(monitor_thread, NULL);
    for (int i = 0; i < rules.number_of_philosophers; i++)
	    pthread_join(philosophers[i].thread, NULL);

    // Cleanup and exit
    for (int i = 0; i < rules.number_of_philosophers; i++)
        pthread_mutex_destroy(&forks[i]);
    
    pthread_mutex_destroy(&rules.print_mutex);
    pthread_mutex_destroy(&rules.death_mutex);
    free(forks);
    free(philosophers);

    return 0;
}
