// 
// 10 тредов, 5 переменных, 5 мютексов, 1 и 2-ой тред делит первую переменную и первый мютекс, 
// 3-4 вторую переменную и второй мютекс и т.д.
// вывести состоиние всех пяти переменных 


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_trh 
{
	int 			share_i;
	pthread_mutex_t mutex;
}			t_trh;

typedef struct s_base
{
	t_trh arr_trh[5];
}			t_base;



void* iteration(t_trh *arr_trh)
{
	int i;
	i = 0;
	while (i++ < 100000)
	{
		// pthread_mutex_lock(&arr_trh->mutex);
		(arr_trh->share_i)++;
		// pthread_mutex_unlock(&arr_trh->mutex);
	}
	return(0);
}

int main()
{

	int		i;
	t_base	base; 
	pthread_t thr[10];
	int cnt_str;

	i = 0;
	while (i < 5)
	{
		base.arr_trh[i].share_i = 0;
		pthread_mutex_init(&base.arr_trh[i].mutex, NULL);
		i++;
	}
	i = 0;
	cnt_str = 0;
	while (i < 10 && cnt_str < 5)
	{
		pthread_create(thr + i, NULL, (void * (*)(void *))iteration, &base.arr_trh[cnt_str]);
		if(i % 2)
			cnt_str++;
		i++;
	}
	while (i > 0)
	{
		i--;
		pthread_join(thr[i], NULL);
	}
	i = 0;
	while(i < 5)
	{
		printf("%d\n", base.arr_trh[i].share_i);
		pthread_mutex_destroy(&base.arr_trh[i].mutex);
		i++;
	}
	return(0);
}

