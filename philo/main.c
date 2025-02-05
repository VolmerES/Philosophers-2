/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdelorme <jdelorme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:18:30 by volmer            #+#    #+#             */
/*   Updated: 2025/02/05 12:43:10 by jdelorme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table		table;
	t_philo		*philos;
	t_mtx		*forks;

	if (argc == 5 || argc == 6)
	{
		philos = ft_malloc_safe(sizeof(t_philo) * ft_atosize_t(argv[1]));
		forks = ft_malloc_safe(sizeof(t_mtx) * ft_atosize_t(argv[1]));
		ft_init_program(&table, philos);
		table.philos->philo_count = ft_atosize_t(argv[1]);
		ft_init_forks(forks, table.philos->philo_count);
		ft_init_philos(philos, &table, forks, argv);
		ft_thread_create(&table, forks);
		ft_destroy_program(NULL, &table, forks);
	}
}
