#ifndef PHILO_H
# define PHILO_H
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define YELLOW	"\e[1;33m"
#define RED		"\e[1;31m"
#define GREEN	"\e[1;32m"
#define VIOLET	"\e[1;35m"
#define BLUE	"\e[1;34m"
#define RESET	"\e[0m"


typedef struct s_init
{
	unsigned int	number_of_phil;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				min_eat_times;
}				t_init;

typedef struct timeval t_timeval;

typedef struct s_pers
{
	struct s_base	*base;
	int				fork1;
	int				fork2;
	t_timeval		last_meal;
	int				count_meal;
	pthread_t 		thr;
	unsigned int	index_phil;
}				t_pers;

typedef struct s_base
{
	pthread_mutex_t	*arr_mutex;
	// pthread_mutex_t	take_fork_mutex;
	pthread_t		supervisor;
	t_pers			*pers;
	t_init			init;
	t_timeval		start_time;
	char			emergency_exit;
}		t_base;

void	*ft_calloc(size_t count, size_t size);


#endif 