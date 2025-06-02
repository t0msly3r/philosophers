#include "../include/philo.h"

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

int check_simulation_stopped(t_rules *rules)
{
    int stopped;

    pthread_mutex_lock(&rules->death_mutex);
    stopped = rules->simulation_stopped;
    pthread_mutex_unlock(&rules->death_mutex);
    return stopped;
}

void stop_simulation(t_rules *rules)
{
    pthread_mutex_lock(&rules->death_mutex);
    rules->simulation_stopped = 1;
    pthread_mutex_unlock(&rules->death_mutex);
}

void *monitor(void *arg)
{
    t_rules *rules = (t_rules *)arg;  // FIX: Recibir directamente t_rules*
    int i;

    while (!check_simulation_stopped(rules))
    {
        i = 0;
        while (i < rules->number_of_philosophers)
        {
            // FIX: Proteger el acceso a last_meal con mutex o variable atómica
            long current_time = get_time_in_ms();
            long last_meal = rules->philos[i].last_meal;
            
            if (current_time - last_meal > rules->time_to_die)
            {
                print_state(&rules->philos[i], "died");
                stop_simulation(rules);
                return (NULL);
            }
            i++;
        }
        usleep(1000); // Sleep for a short duration to avoid busy waiting
    }
    return NULL;
}

int check_times_eaten(t_philo *philo)
{
    if (philo->rules->must_eat == -1)
        return 0; // No eating limit, always return true
    return (philo->eat_count >= philo->rules->must_eat);
}
