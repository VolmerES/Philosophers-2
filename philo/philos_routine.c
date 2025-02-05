/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:46:06 by jdelorme          #+#    #+#             */
/*   Updated: 2025/02/05 21:01:06 by volmer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	ft_print_message("has taken a fork", philo, philo->id);
	if (philo->philo_count == 1)
	{
		ft_usleep(philo->time_to_die);
		ft_mutex_safe(philo->right_fork, UNLOCK);
		return ;
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

void	*ft_philo_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	if (philo->id % 2 == 0)
		ft_usleep(philo->time_to_eat);
	while (!ft_dead_loop(philo))
	{
		ft_philo_eat(philo);
		ft_philo_sleep(philo);
		ft_philo_think(philo);
	}
	return (pointer);
}
