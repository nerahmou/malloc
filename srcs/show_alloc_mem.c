/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   show_alloc_mem.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/19 13:22:49 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/19 17:00:38 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	print_region_info(t_region *region, const char *region_name)
{
	write(1, region_name, 5);
	write(1, " : " HEXA_PREFIX, 5);
	ft_putnbr_base((long)region, HEXA_BASE_STR, HEXA_BASE);
	write(1, "\n", 1);
}

size_t	print_chunks(t_region *region)
{
	size_t		chunk_start;
	size_t		data_size;
	size_t		total;
	t_chunk		*chunk;

	total = 0;
	chunk = GET_FIRST_CHUNK(region);
	while (chunk)
	{
		if (chunk->header.in_use)
		{
			chunk_start = (size_t)CHUNK_DATA(chunk);
			data_size = CHUNK_DATA_SIZE(chunk);
			write(1, HEXA_PREFIX, 2);
			ft_putnbr_base(chunk_start, HEXA_BASE_STR, HEXA_BASE);
			write(1, " - " HEXA_PREFIX, 5);
			ft_putnbr_base(chunk_start + data_size, HEXA_BASE_STR, HEXA_BASE);
			write(1, " : ", 3);
			ft_putnbr_base(data_size, DECI_BASE_STR, DECI_BASE);
			write(1, " octets\n", 8);
			total += data_size;
		}
		chunk = chunk->header.next_size == 0 ? NULL : GET_NEXT_CHUNK(chunk);
	}
	return (total);
}

void	show_alloc_mem()
{
	t_region	*region;
	size_t		total;
	short		i;

	total = 0;
	i = -1;
	write(1, "<<<<<<<<<\n", 10);
	while (g_op[++i].region_name)
	{
		region = *GET_APPROPRIATE_REGION_TYPE(g_op[i].offset);
		while (region)
		{
			print_region_info(region, g_op[i].region_name);
			total += print_chunks(region);
			region = region->next;
		}
	}
	write(1, "Total : ", 8);
	ft_putnbr_base(total, DECI_BASE_STR, DECI_BASE);
	write(1, " octets\n>>>>>>>>>>\n\n", 20);
}
