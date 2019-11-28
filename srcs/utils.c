/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/28 17:19:00 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

t_heap	g_heap = {NULL, NULL, NULL};
t_freed	*g_bins[BINS_NUMBER] = {NULL};

void	show_bins(size_t nb)
{
	t_freed *chunk;
	unsigned short i;

	i = 0;
	ft_putchar('\n');
	while (i < nb && i < BINS_NUMBER)
	{
		ft_printf(BOLD"[" );
		ft_putnbr_base((16) * (i + 1) + 32, DECI_BASE_STR, DECI_BASE);
		ft_printf("]:\t" RES_COLOR);
		chunk = g_bins[i];
		while (chunk)
		{
			ft_printf(BOLD GREEN"{0x" );
			ft_putnbr_base((long)chunk, HEXA_BASE_STR, HEXA_BASE);
			ft_printf("}->" );
			chunk = chunk->next_freed;
		}
		ft_printf(BOLD RED"{NULL}\n" RES_COLOR);
		i++;
	}
}

size_t	next_multiple(size_t size, size_t mult)
{
	return (((size - 1) / mult * mult + mult));
}

size_t	required_size(size_t size, size_t header_size, size_t mult)
{
	return (next_multiple(size + header_size, mult));
}


