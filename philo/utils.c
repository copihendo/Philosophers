#include "philo.h"

void	ft_bzero(void *dst, size_t n)
{
	char	*br;

	br = (char *)dst;
	if (n != 0)
	{	
		while (n != 0)
		{
			*br = 0;
			br++;
			n--;
		}
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*arr;

	if (!size || !count)
	{
		size = 1;
		count = 1;
	}
	arr = malloc(count * size);
	if (!(arr))
		return (NULL);
	ft_bzero(arr, count * size);
	return (arr);
}