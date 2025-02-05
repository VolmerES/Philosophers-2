/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:24:45 by jdelorme          #+#    #+#             */
/*   Updated: 2025/02/05 01:51:48 by volmer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* pthread_create, pthread_join, pthread_mutex_init, 
pthread_mutex_lock, pthread_mutex_unlock*/
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
// gettimeofday
# include <sys/time.h>
# include <limits.h>
# include <errno.h>
# include <stdbool.h>
# include <stdint.h>
# include <string.h>

# define RED     "\x1B[31m"
# define GREEN   "\x1B[32m"
# define BLUE    "\x1B[34m"
# define RESET   "\x1B[0m"
# define PHILO_MAX 200

//          PARAMETROS DE FILO           //
//---------------------------------------//
//?  ./philo   5    800   200   300  [5]
//?  5   ---> Numero de filosofos
//?  800 ---> Tiempo para morir
//?  200 ---> Tiempo para comer
//?  300 ---> Tiempo para dormir
//?  [5] ---> Veces que cada filo debe comer
//--------------------------------------//

typedef pthread_mutex_t	t_mtx;
typedef struct s_table	t_table;
typedef struct s_philo	t_philo;

//-------------ESTRUCTURAS--------------//

//! ESTRUCTURA DE CODIGOS DE OPERACION MUTEX//
typedef enum e_code
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}			t_code;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eat;
	int				meal_count;
	size_t			last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				philo_count;
	int				meal_max;
	int				*dead;
	t_mtx			*right_fork;
	t_mtx			*left_fork;
	t_mtx			*write_lock;
	t_mtx			*dead_lock;
	t_mtx			*meal_lock;
}				t_philo;

typedef struct s_table
{
	int				dead_flag;
	t_mtx			deadlock;
	t_mtx			writelock;
	t_mtx			meallock;
	t_philo			*philos;
}			t_table;




int		ft_usleep(size_t milliseconds);
void	ft_philo_think(t_philo *philo);
void	ft_philo_sleep(t_philo *philo);
void	ft_philo_eat(t_philo *philo);
void	ft_destroy_program(char *error, t_table *table, t_mtx *forks);
void	ft_print_message(char *message, t_philo *philo, int id);
int		ft_philo_dead(t_philo *philo, size_t time_to_die);
int		ft_check_id_dead(t_philo *philos);
int		ft_check_if_all_ate(t_philo *philos);
void	*ft_monitor(void *pointer);
int		ft_dead_loop(t_philo *philo);
void	*ft_philo_routine(void *pointer);
int		ft_thread_create(t_table *table, t_mtx *forks);
int		ft_error_exit(char *error);
size_t	ft_get_current_time(void);
void	ft_error_thread(t_code code, int status);
void	ft_mutex_safe(t_mtx *mutex, t_code code);
void	ft_init_philos(t_philo *philos, t_table *table, t_mtx *forks, char **argv);
void	ft_init_forks(t_mtx *forks, size_t philo_count);
void	ft_init_program(t_table *table, t_philo *philos);
size_t	ft_atosize_t(char *argv);
void	ft_parsing_philo(t_philo *philo, char **argv);
void	ft_error_mutex(int status);

#endif