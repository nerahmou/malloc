/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   show_alloc_mem.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/19 13:22:49 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/06 17:39:13 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	print_region_info(t_region *region)
{
	char	*region_name;

	if (region->max_chunk_size == TINY_MAX_SIZE)
		region_name = "TINY ";
	else if (region->max_chunk_size == SMALL_MAX_SIZE)
		region_name = "SMALL";
	else
		region_name = "LARGE";
	ft_printf(UNDERLINE "" BOLD "%s : " GREEN "" HEXA_PREFIX, region_name);
	ft_putnbr_base((long)region, HEXA_BASE_STR, HEXA_BASE);
	ft_printf("\n" RES_COLOR);
}

size_t	print_chunks(t_region *region)
{
	register t_chunk	*chunk;
	size_t				chunk_start;
	size_t				data_size;
	size_t				total;

	total = 0;
	chunk = (t_chunk*)((size_t)region + sizeof(t_region));
	while (chunk)
	{
		if (chunk->in_use)
		{
			chunk_start = (size_t)&(chunk->data);
			data_size = chunk->size - (sizeof(t_chunk) - 8);
			ft_printf("\t" BOLD "" BLUE "" HEXA_PREFIX);
			ft_putnbr_base(chunk_start, HEXA_BASE_STR, HEXA_BASE);
			ft_printf(RES_COLOR " - " BOLD "" RED "" HEXA_PREFIX);
			ft_putnbr_base(chunk_start + data_size, HEXA_BASE_STR, HEXA_BASE);
			ft_printf(RES_COLOR " : " BOLD);
			ft_putnbr_base(data_size, DECI_BASE_STR, DECI_BASE);
			ft_printf(RES_COLOR "" ITALIC " octets\n" RES_COLOR);
			total += data_size;
		}
		chunk = chunk->next;
	}
	return (total);
}

void	show_alloc_mem(void)
{
	register t_region	*region;
	size_t				total;
	short				i;

	total = 0;
	i = -1;
	ft_putchar('\n');
	while (++i < NB_TYPE_OF_REGION)
	{
		region = *(&(g_heap.tiny_region) + i);
		while (region)
		{
			print_region_info(region);
			total += print_chunks(region);
			region = region->next;
		}
	}
	ft_printf(BOLD "" UNDERLINE "\nTotal : ");
	ft_putnbr_base(total, DECI_BASE_STR, DECI_BASE);
	ft_printf(" octets\n\n" RES_COLOR);
}
