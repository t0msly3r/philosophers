#include "../include/philo.h"

void check_arguments(int argc, char **argv)
{
    int i;
    int j;

    j = 1;
    i = 0;
    if (argc < 5 || argc > 6)
    {
        printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [must_eat]\n");
        exit(EXIT_FAILURE);
    }
    while (j < argc)
    {
        i = 0;
        while (argv[j][i])
        {
            if (argv[j][i] < '0' || argv[j][i] > '9')
            {
                printf("Invalid argument: %s\n", argv[j]);
                exit(EXIT_FAILURE);
            }
            i++;
        }
        j++;
    }
}

void ft_cleanup(t_rules *rules, t_philo *philosophers, pthread_mutex_t *forks)
{
    int i;

    i = 0;
    while (i < rules->number_of_philosophers)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
    pthread_mutex_destroy(&(rules)->print_mutex);
    pthread_mutex_destroy(&(rules)->death_mutex);
    free(forks);
    free(philosophers);
}

long long get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void print_state(t_philo *philo, char *msg)
{
	long long timestamp;

	pthread_mutex_lock(&philo->rules->print_mutex);
	if (!check_simulation_stopped(philo->rules))  // FIX: Solo imprimir si la simulación no se ha detenido
	{
		timestamp = get_time_in_ms() - philo->rules->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

int check_times_eaten(t_philo *philo)
{
    if (philo->rules->must_eat == -1)
        return 0; // No eating limit, always return true
    return (philo->eat_count >= philo->rules->must_eat);
}
