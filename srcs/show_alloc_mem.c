/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   show_alloc_mem.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/19 13:22:49 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 11:09:50 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	print_region_info(t_region *region, unsigned char i)
{
	ft_printf(UNDERLINE BOLD "%i : " GREEN HEXA_PREFIX, i);
	ft_putnbr_base((long)region, HEXA_BASE_STR, HEXA_BASE);
	ft_printf("\n{eoc}");
	ft_printf("%i : ", i);
	ft_putnbr_base((long)region, HEXA_BASE_STR, HEXA_BASE);
	ft_printf("\n");
}

size_t	print_chunks(t_region *region)
{
	size_t		chunk_start;
	size_t		data_size;
	size_t		total;
	t_chunk		*chunk;

	total = 0;
	chunk = FIRST_CHUNK(region);
	while (chunk)
	{
		if (chunk->in_use)
		{
			chunk_start = (size_t)CHUNK_DATA(chunk);
			data_size = DATA_SIZE(chunk);
			ft_printf( "\t" BOLD BLUE HEXA_PREFIX);
			ft_putnbr_base(chunk_start, HEXA_BASE_STR, HEXA_BASE);
			ft_printf(RES_COLOR " - " BOLD RED HEXA_PREFIX);
			ft_putnbr_base(chunk_start + data_size, HEXA_BASE_STR, HEXA_BASE);
			ft_printf(RES_COLOR " : " BOLD);
			ft_putnbr_base(data_size, DECI_BASE_STR, DECI_BASE);
			ft_printf(RES_COLOR ITALIC " octets\n" RES_COLOR);
			chunk_start = (size_t)CHUNK_DATA(chunk);
			data_size = DATA_SIZE(chunk);
			ft_printf( "\t" );
			ft_putnbr_base(chunk_start, HEXA_BASE_STR, HEXA_BASE);
			ft_printf(" - ");
			ft_putnbr_base(chunk_start + data_size, HEXA_BASE_STR, HEXA_BASE);
			ft_printf(" : ");
			ft_putnbr_base(data_size, DECI_BASE_STR, DECI_BASE);
			ft_printf(" octets\n");
			total += data_size;
		}
		//chunk = chunk->next_size == 0 ? NULL : NEXT_CHUNK(chunk);
		chunk = chunk->next;
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
	ft_putchar('\n');
	while (++i < 3)
	{
		region = *(&g_heap.tiny_region + i); //*APPROPRIATE_REGION_TYPE(g_op[i].offset);
		while (region)
		{
			print_region_info(region, i);
			total += print_chunks(region);
			region = region->next;
		}
	}

	ft_printf("\nTotal : ");
	ft_putnbr_base(total, DECI_BASE_STR, DECI_BASE);
	ft_printf(" octets\n\n");
	ft_printf(BOLD UNDERLINE"\nTotal : ");
	ft_putnbr_base(total, DECI_BASE_STR, DECI_BASE);
	ft_printf(" octets\n\n" RES_COLOR);
}
