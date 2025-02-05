/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdelorme <jdelorme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:44:49 by jdelorme          #+#    #+#             */
/*   Updated: 2025/02/05 12:52:57 by jdelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = ft_get_current_time();
	while ((ft_get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

size_t	ft_get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL))
		ft_error_exit(RED "Error: gettimeofday failed\n" RESET);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_destroy_program(char *error, t_table *table, t_mtx *forks)
{
	int	i;

	i = 0;
	if (error)
	{
		write(2, error, ft_strlen(error));
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
	free(table->philos);
	table->philos = NULL;
	free(forks);
	forks = NULL;
}

void	ft_print_message(char *message, t_philo *philo, int id)
{
	size_t	time;

	ft_mutex_safe(philo->write_lock, LOCK);
	time = ft_get_current_time() - philo->start_time;
	if (!ft_dead_loop(philo))
	{
		printf("%lu %d %s\n", time, id, message);
	}
	ft_mutex_safe(philo->write_lock, UNLOCK);
}
