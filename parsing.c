/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdelorme <jdelorme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:48:23 by jdelorme          #+#    #+#             */
/*   Updated: 2025/02/05 12:33:59 by jdelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_is_positive(char c)
{
	if (c == '+')
		return (1);
	return (0);
}

size_t	ft_atosize_t(char *argv)
{
	size_t	nbr;
	int		i;

	nbr = 0;
	i = 0;
	while (argv[i])
	{
		while (ft_is_positive(argv[i]))
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
