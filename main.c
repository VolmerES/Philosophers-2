/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:18:30 by volmer            #+#    #+#             */
/*   Updated: 2025/02/05 01:49:33 by volmer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_usleep(size_t milliseconds)
{
	size_t start;

	start = ft_get_current_time();
	while (ft_get_current_time() - start < milliseconds)
		usleep(500);
	return (0);	
}










void	ft_philo_think(t_philo *philo)
{
	ft_print_message("is thinking", philo, philo->id);
}

void	ft_philo_sleep(t_philo *philo)
{
	ft_print_message("is sleeping", philo, philo->id);
	ft_usleep(philo->time_to_sleep);
}

void	ft_philo_eat(t_philo *philo)
{
	ft_mutex_safe(philo->right_fork, LOCK);
	if (philo->philo_count == 1)
	{
		ft_usleep(philo->time_to_die);
		ft_mutex_safe(philo->right_fork, UNLOCK);
		return;
	}

	ft_mutex_safe(philo->left_fork, LOCK);
	
	ft_print_message("has taken a fork", philo, philo->id);
	philo->eat = 1;
	ft_print_message("is eating", philo, philo->id);
	ft_mutex_safe(philo->meal_lock, LOCK);
	philo->last_meal = ft_get_current_time();
	philo->meal_count++;
	ft_mutex_safe(philo->meal_lock, UNLOCK);
	ft_usleep(philo->time_to_eat);
	philo->eat = 0;
	ft_mutex_safe(philo->left_fork, UNLOCK);
	ft_mutex_safe(philo->right_fork, UNLOCK);
}





void ft_destroy_program(char *error, t_table *table, t_mtx *forks)
{
    int i;

    i = 0;
	if (error)
	{
		write(2, error, strlen(error));
		write(2, "\n", 1);
	}
    ft_mutex_safe(&table->writelock, DESTROY);
    ft_mutex_safe(&table->meallock, DESTROY);
    ft_mutex_safe(&table->deadlock, DESTROY);
	while (i < table->philos[0].philo_count)
	{
		ft_mutex_safe(&forks[i], DESTROY);
		i++;
	}
}


void	ft_print_message(char *message, t_philo *philo, int id)
{
	size_t time;

	ft_mutex_safe(philo->write_lock, LOCK);
	time = ft_get_current_time() - philo->start_time;
	if (!ft_dead_loop(philo))
	{
		printf("%lu %d %s\n", time, id, message);
	}
	ft_mutex_safe(philo->write_lock, UNLOCK);
	
}

int	ft_philo_dead(t_philo *philo, size_t time_to_die)
{
	ft_mutex_safe(philo->meal_lock, LOCK);
	if (ft_get_current_time() - philo->last_meal >= time_to_die && philo->eat == 0)
	{
		return (ft_mutex_safe(philo->meal_lock, UNLOCK), 1);
	}
	ft_mutex_safe(philo->meal_lock, UNLOCK);
	return (0);
}

int	ft_check_if_dead(t_philo *philos)
{
	int i;

	i = 0;
	while (i < philos[0].philo_count)
	{
		if (ft_philo_dead(&philos[i], philos[i].time_to_die))
		{
			ft_print_message("died", &philos[i], philos[i].id);
			ft_mutex_safe(philos[0].dead_lock, LOCK);
			*philos->dead = 1;
			ft_mutex_safe(philos[0].dead_lock, UNLOCK);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_check_if_all_ate(t_philo *philos)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].meal_max == -1)
		return (0);
	while (i < philos[0].philo_count)
	{
		ft_mutex_safe(philos[i].meal_lock, LOCK);
		if (philos[i].meal_count >= philos[i].meal_max)
			finished_eating++;
		ft_mutex_safe(philos[i].meal_lock, UNLOCK);
		i++;
	}
	if (finished_eating == philos[0].philo_count)
	{
		ft_mutex_safe(philos[0].dead_lock, LOCK);
		*philos->dead = 1;
		ft_mutex_safe(philos[0].dead_lock, UNLOCK);
		return (1);
	}
	return (0);
}

void *ft_monitor(void *pointer)
{
	t_philo *philos;

	philos = (t_philo *)pointer;
	while (1)
	{
		if (ft_check_if_dead(philos) == 1 || ft_check_if_all_ate(philos) == 1)
			break ;
	}
	return (pointer);
}






int		ft_dead_loop(t_philo *philo)
{
	ft_mutex_safe(philo->dead_lock, LOCK);
	if (*philo->dead == 1)
	{
		ft_mutex_safe(philo->dead_lock, UNLOCK);
		return (1);
	}
	ft_mutex_safe(philo->dead_lock, UNLOCK);
	return (0);
}

void	*ft_philo_routine(void *pointer)
{
	t_philo *philo;
	
	philo = (t_philo *)pointer;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!ft_dead_loop(philo))
	{
		ft_philo_eat(philo);
		ft_philo_sleep(philo);
		ft_philo_think(philo);
	}
	return (pointer);
}

int	ft_thread_create(t_table *table, t_mtx *forks)
{
	pthread_t	observer;
	int			i;
	
	if (pthread_create(&observer, NULL, &ft_monitor, table->philos) != 0)
		ft_destroy_program("Thread creation error", table, forks);
	i = 0;
	while (i < table->philos[0].philo_count)
	{
		if (pthread_create(&table->philos[i].thread, NULL, &ft_philo_routine, &table->philos[i]) != 0)
			ft_destroy_program("Thread creation error", table, forks);
		i++;
	}
	i = 0;
	if (pthread_join(observer, NULL) != 0)
	{
		ft_destroy_program("Thread join error", table, forks);
	}
	while (i < table->philos[0].philo_count)
	{
		if (pthread_join(table->philos[i].thread, NULL) != 0)
			ft_destroy_program("Thread join error", table, forks);
		i++;
	}
	return (0);
}




int	ft_error_exit(char *error)
{
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}






size_t	ft_get_current_time(void)
{
	struct timeval time;
	
	if (gettimeofday(&time, NULL))
		ft_error_exit(RED "Error: gettimeofday failed\n" RESET);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}



void	ft_error_thread(t_code code, int status)
{
	if (status == 0)
		return ;
	else if ((code == DETACH || code == JOIN) && status == EINVAL)
		ft_error_exit(RED "Error: Thread is not joinable or another"
			" thread is already waiting to join with this thread\n" RESET);
	else if ((code == DETACH || code == JOIN) && status == ESRCH)
		ft_error_exit(RED "Error: No thread with the ID thread"
			" could be found\n" RESET);
	else if (code == JOIN && status == EDEADLK)
		ft_error_exit(RED "Error: A deadlock was detected\n" RESET);
	else if (code == CREATE && status == EPERM)
		ft_error_exit(RED "Error: No permission to set the"
			" scheduling policy"
			" and parameters specified in attr.\n" RESET);
}

void	ft_error_mutex(int status)
{
	if (status == 0)
		return ;
	if (status == EINVAL)
		ft_error_exit(RED "Error: The value specified"
			" by mutex is invalid\n" RESET);
	else if (status == EBUSY)
		ft_error_exit(RED "Error: The implementation has detected an attempt"
			" to destroy the object referenced by mutex while it is locked or"
			" referenced by another thread\n" RESET);
	else if (status == EAGAIN)
		ft_error_exit(RED "Error: The system lacked the necessary resources"
			" to initialise another mutex\n" RESET);
	else if (status == EDEADLK)
		ft_error_exit(RED "Error: A deadlock condition was detected or the"
			" current thread already owns the mutex\n" RESET);
	else if (status == EPERM)
		ft_error_exit(RED "Error: The current thread does not"
			" own the mutex\n" RESET);
	else
		ft_error_exit(RED "Error: Mutex operation not found\n" RESET);
}

void	ft_mutex_safe(t_mtx *mutex, t_code code)
{
	if (code == INIT)
		pthread_mutex_init(mutex, NULL);
	else if (code == UNLOCK)
		pthread_mutex_unlock(mutex);
	else if (code == LOCK)
		pthread_mutex_lock(mutex);
	else if (code == DESTROY)
		pthread_mutex_destroy(mutex);
	else
		ft_error_exit(RED "Error: Mutex operation not found\n" RESET);
}






void	ft_init_philos(t_philo *philos, t_table *table, t_mtx *forks, char **argv)
{
	size_t i;
	
	i = 0;
	while (i < ft_atosize_t(argv[1]))
	{
		philos[i].id = i + 1;
		philos[i].eat = 0;
		philos[i].meal_count = 0;
		ft_parsing_philo(&philos[i], argv);
		philos[i].start_time = ft_get_current_time();
		philos[i].last_meal = ft_get_current_time();
		philos[i].write_lock = &table->writelock;
		philos[i].dead_lock = &table->deadlock;
		philos[i].meal_lock = &table->meallock;
		philos[i].dead = &table->dead_flag;
		philos[i].left_fork = &forks[i];
		if (i == 0)
			philos[i].right_fork = &forks[philos[i].philo_count - 1];
		else
			philos[i].right_fork = &forks[i - 1];
		i++;
	}
}

void	ft_init_forks(t_mtx *forks, size_t philo_count)
{
	size_t i;
	
	i = 0;
	while (i < philo_count)
	{
		ft_mutex_safe(&forks[i], INIT);
		i++;
	}
}

void ft_init_program(t_table *table, t_philo *philos)
{
	table->dead_flag = 0;
	table->philos = philos;
	ft_mutex_safe(&table->writelock, INIT);
	ft_mutex_safe(&table->deadlock, INIT);
	ft_mutex_safe(&table->meallock, INIT);
}






static int ft_is_negative_or_positive(char c)
{
	if (c == '-' || c == '+')
		return (1);
	return (0);
}

size_t	ft_atosize_t(char *argv)
{
	size_t nbr;
	int i;
	
	nbr = 0;
	i = 0;
	while (argv[i])
	{
		while (ft_is_negative_or_positive(argv[i]))
			i++;
			
		if (argv[i] == '-')
			ft_error_exit(RED "Error: Wrong parameters\n"
				BLUE "Please note, negatives numbers are not allowed" RESET);
		else if (argv[i] < '0' || argv[i] > '9')
			ft_error_exit(RED "Error: Wrong parameters\n"
				BLUE "Please note, only numbers allowed" RESET);
		nbr = nbr * 10 + (argv[i] - '0');
		i++;
		if (nbr > SIZE_MAX / 10 || (nbr == SIZE_MAX / 10
				&& (unsigned long)(argv[i] - '0') > SIZE_MAX % 10))
			ft_error_exit(RED "Error: Wrong parameters\n"
				BLUE "Please note, numbers are too big" RESET);
	}
	return (nbr);
}

void	ft_parsing_philo(t_philo *philo, char **argv)
{
	philo->philo_count = ft_atosize_t(argv[1]);
	philo->time_to_die = ft_atosize_t(argv[2]);
	philo->time_to_eat = ft_atosize_t(argv[3]);
	philo->time_to_sleep = ft_atosize_t(argv[4]);
	if (argv[5])
		philo->meal_max = ft_atosize_t(argv[5]);
	else
		philo->meal_max = -1;
}

int	main (int argc, char **argv)
{
	t_table		table;
	t_philo		philos[PHILO_MAX];
	t_mtx		forks[PHILO_MAX];
	
	if (argc == 5 || argc == 6)
	{
		ft_init_program(&table, philos);
		table.philos->philo_count = ft_atosize_t(argv[1]);
		ft_init_forks(forks, table.philos->philo_count);
		ft_init_philos(philos, &table, forks, argv);
		ft_thread_create(&table, forks);
		ft_destroy_program(NULL, &table, forks);
	}
}
