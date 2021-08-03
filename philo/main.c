#include "philo.h"
#include <math.h>
// void	ft_exit(t_base *base, const char *messege)
// {
// 	if (!messege)
// 		exit (0);
// 	ft_putendl_fd("Error", 2);
// 	ft_putendl_fd(messege, 2);
// 	exit(1);
// }


int	ft_init_argv(t_base *base, int __attribute__((unused))argc, char __attribute__((unused))*argv[])
{
	base->init.number_of_phil = 5;
	base->init.time_to_die = 800;
	base->init.time_to_eat = 20000;
	base->init.time_to_sleep = 200;
	base->init.min_eat_times = 20;
	return (0);
}

int	ft_init_person(t_base *base)
{
	unsigned int i;
	// base->pers;
	// выделить память под все эдементы с неопределенным кол-вом филосовоф
	// base->pers, base->arr_mutex
	base->pers = ft_calloc(base->init.number_of_phil, sizeof(t_pers)); // обезопасить 
	base->arr_mutex = ft_calloc(base->init.number_of_phil, sizeof(pthread_mutex_t));
	base->emergency_exit = 0;
	// персональные индексы. Кто у кого берет вилку
	// 0-ый берет 0(своя) и 1(сосед) вилку
	i = 0;
	while(i < base->init.number_of_phil)
	{	
		int buf;
		gettimeofday(&base->pers[i].last_meal, NULL);
		base->pers[i].count_meal = 0;
		base->pers[i].base = base;
		base->pers[i].index_phil = i;
		base->pers[i].fork1 = i;
		base->pers[i].fork2 = (i + 1) % base->init.number_of_phil; // check the test
		if (i + 1 == base->init.number_of_phil)
		{
			buf = base->pers[i].fork1;
			base->pers[i].fork1 = base->pers[i].fork2;
			base->pers[i].fork2 = buf;
		}//	base->pers[i].fork1 = i - 1;
		// инициальизировать все мютексы
		pthread_mutex_init(&base->arr_mutex[i], NULL);
		i++;
	}
	// pthread_mutex_init(&base->take_fork_mutex, NULL);
	gettimeofday(&base->start_time, NULL); // заглушка
	return(0);
}

void	ft_ending(t_base *base)
{
	// завершение процесса. удалить память, закрыть мютексы. 
	unsigned int i;

	i = 0;
	while (i < base->init.number_of_phil)
	{
		pthread_mutex_destroy(&base->arr_mutex[i]);
		i++;
	}
	free(base->arr_mutex);
	free(base->pers);

}

unsigned int deltatime(t_timeval tv1)
{
	t_timeval tvnow;
	// t_timeval delta;

	gettimeofday(&tvnow, NULL);
	// delta.tv_sec = tvnow.tv_sec - tv1->tv_sec;
	// delta.tv_usec = tvnow.tv_usec - tv1->tv_usec;
	// if(delta.tv_usec<0) { delta.tv_sec--; delta.tv_usec+=1000000; }
	// return(delta.tv_sec*1000+delta.tv_usec/1000);
	return((tvnow.tv_sec - tv1.tv_sec) * 1000 + (tvnow.tv_usec - tv1.tv_usec) / 1000);
	// else 
	// 	return((tvnow.tv_sec - tv1->tv_sec) * 1000 + (tv1->tv_usec - tvnow.tv_usec) / 1000);
}


void ft_status(t_pers *pers, char *message)
{
	unsigned long time;

	time = deltatime(pers->base->start_time);
	if (pers->base->emergency_exit == 0)
		printf("%ld \e[38;5;%dm%d  %s\n", time, pers->index_phil + 5,pers->index_phil + 1, message);
}


void	*ft_supervisor(t_base *base)
{
	unsigned int i, time;

	while(1)
	{
		i = 0;
		while (i < base->init.number_of_phil)
		{
			time = deltatime(base->pers[i].last_meal);
			if (time > base->init.time_to_die && base->pers[i].count_meal != base->init.min_eat_times)
			{
				ft_status(&base->pers[i], RED"died"RESET);
				base->emergency_exit = 1;
				printf("time = %u\n", time);
				return (0);
			}
		}
	}
}

void	ft_usleep(t_pers *pers, unsigned int time_ms)
{
	// printf("%p", &pers->last_meal);
	unsigned int time_before_died;
	time_before_died = pers->base->init.time_to_die - deltatime(pers->last_meal);
	if (time_ms >  time_before_died)
		time_ms = time_before_died + 10;
	printf("time_ms! = %u\n", time_ms);
	usleep(time_ms * 1000);

}

void	ft_sleeping(t_pers *pers)
{
	ft_status(pers, BLUE"is sleeping"RESET);
	ft_usleep(pers, pers->base->init.time_to_sleep);
}

void	ft_thinking(t_pers *pers)
{
	ft_status(pers, VIOLET"is thinking"RESET);
}

void	ft_eating(t_pers *pers)
{
	// gettimeodfay(tv1);
	// pthread_mutex_lock(&pers->base->take_fork_mutex);
	pthread_mutex_lock(&pers->base->arr_mutex[pers->fork1]);
	ft_status(pers, YELLOW"has taken a fork"RESET);
	pthread_mutex_lock(&pers->base->arr_mutex[pers->fork2]);
	ft_status(pers, YELLOW"has taken a fork"RESET);
	// pthread_mutex_unlock(&pers->base->take_fork_mutex);
	gettimeofday(&pers->last_meal, NULL);
	ft_status(pers, GREEN"is eating"RESET);
	ft_usleep(pers, pers->base->init.time_to_eat);
	pthread_mutex_unlock(&pers->base->arr_mutex[pers->fork1]);
	pthread_mutex_unlock(&pers->base->arr_mutex[pers->fork2]);
	// gettimeodfay(tv2);

}


void	*ft_philo(t_pers *pers)
{
	while(pers->base->emergency_exit == 0 && pers->base->init.number_of_phil != 1 && (pers->count_meal < pers->base->init.min_eat_times || pers->base->init.min_eat_times == -1))
	{
		ft_eating(pers);
		pers->count_meal++;
		if (pers->count_meal == pers->base->init.min_eat_times)
			return (0);
		if (pers->base->emergency_exit == 1)
			return(0);
		ft_sleeping(pers);
		if (pers->base->emergency_exit == 1)
			return(0);
		ft_thinking(pers);
	}
	return(0);
}

int main(int argc, char *argv[])
{
	t_base base;
	unsigned int i;

	ft_init_argv(&base, argc, argv);
	ft_init_person(&base);
	i = 0;
	// запустить треды
	pthread_create(&base.supervisor, NULL, (void * (*)(void *))ft_supervisor, &base);
	pthread_detach(base.supervisor);
	while (i < base.init.number_of_phil)
	{
		pthread_create(&base.pers[i].thr, NULL, (void * (*)(void *))ft_philo, &base.pers[i]);
		usleep(3000); // costb|l
		i++;
	}
	while(i > 0)
	{
		i--;
// джоин. дождаться завершения тредов
		pthread_join(base.pers[i].thr, NULL);
	}
	ft_ending(&base);
// тред супервизора
// завершение процесса. удалить память, закрыть мютексы. 

}