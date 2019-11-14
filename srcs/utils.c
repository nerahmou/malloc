/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/14 15:26:24 by nerahmou    ###    #+. /#+    ###.fr     */
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

void ft_putnbr_base(size_t nbr, const char *base, size_t base_len)
{
	if (nbr >= base_len)
	{
		ft_putnbr_base(nbr / base_len, base, base_len);
		ft_putnbr_base(nbr % base_len, base, base_len);
	}
	else
		write(1, &(base[nbr]), 1);
}

void	print_region_info(t_region *region, const char *region_name)
{
	write(1, region_name, 5);
	write(1, " : " HEXA_PREFIX, 5);
	ft_putnbr_base((long)region, HEXA_BASE_STR, HEXA_BASE);
	write(1, "\n", 1);
}

size_t	print_chunks(t_region *region, bool is_large)
{
	size_t		chunk_start;
	size_t		chunk_data_size;
	size_t		total;
	t_chunk		*chunk;

	total = 0;
	chunk = GET_FIRST_CHUNK(region);
	while (chunk)
	{
		if (is_large == true || chunk->in_use)
		{
			chunk_start = is_large ? (size_t)chunk : (size_t)CHUNK_DATA(chunk);
			chunk_data_size = is_large ? AVAILABLE_SPACE(region) : CHUNK_DATA_SIZE(chunk);
			write(1, HEXA_PREFIX, 2);
			ft_putnbr_base(chunk_start, HEXA_BASE_STR, HEXA_BASE);
			write(1, " - " HEXA_PREFIX, 5);
			ft_putnbr_base(chunk_start + chunk_data_size, HEXA_BASE_STR, HEXA_BASE);
			write(1, " : ", 3);
			ft_putnbr_base(chunk_data_size, DECI_BASE_STR, DECI_BASE);
			write(1, " octets\n", 8);
			total += chunk_data_size;
		}
		total += chunk_data_size;
		chunk = GET_NEXT_CHUNK(((is_large || !chunk->next_size) ? NULL : chunk));
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
		region = *GET_APPROPRIATE_region_TYPE(g_op[i].offset);
		while (region)
		{
			print_region_info(region, g_op[i].region_name);
			total += print_chunks(region, g_op[i].is_large);
			region = region->next;
		}
	}
	write(1, "Total : ", 8);
	ft_putnbr_base(total, DECI_BASE_STR, DECI_BASE);
	write(1, " octets\n>>>>>>>>>>\n\n", 20);
}

void	show_bins()
{
	t_chunk *chunk;
	short i;

	i = 0;
	write(1, "<<<<<<<<<<\n", 11);
	while (i < BINS_NUMBER)
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
