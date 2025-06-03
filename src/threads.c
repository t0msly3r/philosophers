#include "../include/philo.h"

void *monitor(void *arg)
{
    t_rules *rules = (t_rules *)arg;  // FIX: Recibir directamente t_rules*
    int i;

    while (!check_simulation_stopped(rules))
    {
        i = 0;
        while (i < rules->number_of_philosophers)
        {
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

static void take_fork(t_philo *philo)
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

static void left_fork(t_philo *philo)
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
}

void    *philosopher(void *arg)
{
    t_philo	*philo;

	philo = (t_philo *)arg;
	while (!check_simulation_stopped(philo->rules) && !check_times_eaten(philo))
	{
		take_fork(philo);
		// Simulate eating
        print_state(philo, "is eating");
        philo->eat_count++;
        philo->last_meal = get_time_in_ms(); // Replace with actual time logic
        usleep(philo->rules->time_to_eat * 1000); // Simulate eating time

        left_fork(philo);
		// Simulate thinking or sleeping
        print_state(philo, "is sleeping");
        usleep(philo->rules->time_to_sleep * 1000); // Simulate sleeping time
        print_state(philo, "is thinking");
	}
	return (NULL);
}
