/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/22 17:35:39 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_heap	g_heap = {NULL, NULL, NULL};

t_chunk	*g_bins[/*BINS_NUMBER*/255] = {0};

t_op g_op[4] = {
	{TINY_MAX_SIZE, TINY_REGION_SIZE, TINY_region_OFFSET, false, "TINY "},
	{SMALL_MAX_SIZE, SMALL_REGION_SIZE, SMALL_region_OFFSET, false, "SMALL"},
	{LARGE_MAX_SIZE, LARGE_REGION_SIZE, LARGE_region_OFFSET, true, "LARGE"},
	{0, 0, 0, NULL, NULL}
};

void	show_bins(size_t nb)
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
