/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/04 14:31:51 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

t_heap	g_heap = {NULL, NULL, NULL};
t_chunk	*g_bins[BINS_NUMBER] = {NULL};

void		show_bins(size_t nb)
{
	unsigned short	i;
	t_chunk			*chunk;

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
			ft_printf(BOLD GREEN"{0x");
			ft_putnbr_base((long)chunk, HEXA_BASE_STR, HEXA_BASE);
			ft_printf("}->");
			chunk = chunk->data;
		}
		ft_printf(BOLD RED"{NULL}\n" RES_COLOR);
		i++;
	}
}

size_t		required_size(size_t size, size_t header_size, size_t mult)
{
	return (next_multiple(size + header_size, mult));
}

bool		ptr_in_region(void *ptr, t_region *region, size_t reg_size)
{
	return (ptr > (void*)region && (size_t)ptr <= (size_t)region + reg_size);
}

t_region	*is_valid_ptr(void *ptr)
{
	unsigned char	i;
	t_region		*region;
	t_chunk			*chunk;

	i = -1;
	while (++i < NB_TYPE_OF_REGION)
	{
		region = *(TINY_REGION_HEAD + i);
		while (region)
		{
			if (ptr_in_region(ptr, region, region->size))
			{
				chunk = (t_chunk*)((size_t)region + REG_HEAD_SIZE);
				while (chunk->size)
				{
					if (&(chunk->data) == ptr)
						return (region);
					chunk = chunk->next;
				}
			}
			region = region->next;
		}
	}
	return (NULL);
}
