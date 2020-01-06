/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/06 17:08:54 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

t_heap	g_heap = {NULL, NULL, NULL};
t_chunk	*g_bins[BINS_NUMBER] = {NULL};

void		show_bins(size_t nb)
{
	register unsigned short	i;
	t_chunk					*chunk;

	i = 0;
	ft_putchar('\n');
	while (i < nb && i < BINS_NUMBER)
	{
		ft_printf(BOLD "[");
		ft_putnbr_base((16) * (i + 1) + 32, DECI_BASE_STR, DECI_BASE);
		ft_printf("]:\t" RES_COLOR);
		chunk = g_bins[i];
		while (chunk)
		{
			ft_printf(BOLD "" GREEN "{0x");
			ft_putnbr_base((long)chunk, HEXA_BASE_STR, HEXA_BASE);
			ft_printf("}->");
			chunk = chunk->data;
		}
		ft_printf(BOLD "" RED "{NULL}\n" RES_COLOR);
		i++;
	}
}

size_t		required_size(size_t size, size_t header_size, size_t mult)
{
	size_t required;

	required = next_multiple(size + header_size, mult);
	if (size < required)
		return (required);
	return (0);
}

bool		ptr_in_region(void *ptr, t_region *region, size_t reg_size)
{
	return (ptr > (void*)region && (size_t)ptr <= (size_t)region + reg_size);
}

t_region	*is_valid_ptr(void *ptr)
{
	unsigned char		i;
	register t_region	*region;
	register t_chunk	*chunk;

	i = -1;
	while (++i < NB_TYPE_OF_REGION)
	{
		region = *(&(g_heap.tiny_region) + i);
		while (region)
		{
			if (ptr_in_region(ptr, region, region->size))
			{
				chunk = (t_chunk*)((size_t)region + sizeof(t_region));
				while (chunk)
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

bool		unused_region(t_region *region)
{
	register t_chunk	*chunk;

	chunk = (t_chunk*)((size_t)region + sizeof(t_region));
	while (chunk && chunk->in_use == false)
		chunk = chunk->next;
	return (chunk == 0);
}
