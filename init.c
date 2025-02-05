/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdelorme <jdelorme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:47:25 by jdelorme          #+#    #+#             */
/*   Updated: 2025/02/05 12:38:29 by jdelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_thread_create(t_table *table, t_mtx *forks)
{
	pthread_t	observer;
	int			i;

	if (pthread_create(&observer, NULL, &ft_monitor, table->philos) != 0)
		ft_destroy_program("Thread creation error", table, forks);
	i = 0;
	while (i < table->philos[0].philo_count)
	{
		if (pthread_create(&table->philos[i].thread, NULL,
				&ft_philo_routine, &table->philos[i]) != 0)
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

void	ft_init_philos(t_philo *philos, t_table *table,
	t_mtx *forks, char **argv)
{
	size_t	i;

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
	size_t	i;

	i = 0;
	while (i < philo_count)
	{
		ft_mutex_safe(&forks[i], INIT);
		i++;
	}
}

void	ft_init_program(t_table *table, t_philo *philos)
{
	table->dead_flag = 0;
	table->philos = philos;
	ft_mutex_safe(&table->writelock, INIT);
	ft_mutex_safe(&table->deadlock, INIT);
	ft_mutex_safe(&table->meallock, INIT);
}
