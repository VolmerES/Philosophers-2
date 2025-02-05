/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdelorme <jdelorme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:53:38 by jdelorme          #+#    #+#             */
/*   Updated: 2025/02/05 11:54:27 by jdelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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