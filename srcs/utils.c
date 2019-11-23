/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/23 19:56:09 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_heap	g_heap = {NULL, NULL, NULL};

//t_chunk	*g_bins[/*BINS_NUMBER*/255] = {0};

/*void	show_bins(size_t nb)
{
	t_chunk *chunk;
	unsigned char i;

	i = 0;
	ft_putchar('\n');
	while (i < nb && i < 255)
	{
		ft_printf(BOLD"[" );
		ft_putnbr_base((ALIGNEMENT) * (i + 1) + 16, DECI_BASE_STR, DECI_BASE);
		ft_printf("]:\t" RES_COLOR);
		chunk = g_bins[i];
		while (chunk)
		{
			ft_printf(BOLD GREEN"{0x" );
			ft_putnbr_base((long)chunk, HEXA_BASE_STR, HEXA_BASE);
			ft_printf("}->" );
			chunk = chunk->next_free;
		}
		ft_printf(BOLD RED"{NULL}\n" RES_COLOR);
		i++;
	}
}
*/
