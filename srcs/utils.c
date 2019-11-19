/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/19 17:00:43 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_heap	g_heap = {NULL, NULL, NULL};

t_chunk	*g_bins[BINS_NUMBER] = {NULL};

t_op g_op[4] = {
	{TINY_MAX_SIZE, TINY_REGION_SIZE, TINY_region_OFFSET, false, "TINY "},
	{SMALL_MAX_SIZE, SMALL_REGION_SIZE, SMALL_region_OFFSET, false, "SMALL"},
	{LARGE_MAX_SIZE, LARGE_REGION_SIZE, LARGE_region_OFFSET, true, "LARGE"},
	{0, 0, 0, NULL, NULL}
};

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char*)s)[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

void*	ft_memcpy(void *s1, const void *s2, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		((unsigned char*)s1)[i] = ((unsigned char*)s2)[i];
		i++;
	}
	return (s1);
}

/*void ft_putnbr_base(size_t nbr, const char *base, size_t base_len)
{
	if (nbr >= base_len)
	{
		ft_putnbr_base(nbr / base_len, base, base_len);
		ft_putnbr_base(nbr % base_len, base, base_len);
	}
	else
		write(1, &(base[nbr]), 1);
}
*/
void	show_bins(size_t type)
{
	t_chunk *chunk;
	unsigned char i;

	i = 0;
	write(1, "<<<<<<<<<<\n", 11);
	while (i < type)
	{
		write(1, "[", 1);
		ft_putnbr_base((ALIGNEMENT) * (i + 1) + 16, DECI_BASE_STR, DECI_BASE);
		write(1, "] : ", 4);
		chunk = g_bins[i];
		while (chunk)
		{
			write(1, "{0x", 3);
			ft_putnbr_base((long)chunk, HEXA_BASE_STR, HEXA_BASE);
			write(1, "}->", 3);
			chunk = chunk->next_free;
		}
		write(1, "{NULL}\n", 7);
		i++;
	}
	write(1, ">>>>>>>>>>\n", 11);
}
