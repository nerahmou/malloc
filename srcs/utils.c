/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/12 13:29:44 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_heap	g_heap = {NULL, NULL, NULL};

t_chunk	*g_bins[BINS_NUMBER] = {NULL};

t_op g_op[4] = {
	{TINY_MAX_SIZE, TINY_SEGMENT_SIZE, TINY_SEGMENT_OFFSET, false, "TINY "},
	{SMALL_MAX_SIZE, SMALL_SEGMENT_SIZE, SMALL_SEGMENT_OFFSET, false, "SMALL"},
	{LARGE_MAX_SIZE, LARGE_SEG_SIZE, LARGE_SEGMENT_OFFSET, true, "LARGE"},
	{0, 0, 0, NULL, NULL}
};

static void ft_putnbr_base(size_t nbr, const char *base, size_t base_len)
{
	if (nbr >= base_len)
	{
		ft_putnbr_base(nbr / base_len, base, base_len);
		ft_putnbr_base(nbr % base_len, base, base_len);
	}
	else
		write(1, &(base[nbr]), 1);
}

void	print_segment_info(t_segment *segment, const char *segment_name)
{
	write(1, segment_name, 5);
	write(1, " : ", 3);
	ft_putnbr_base((long)segment, HEXA_BASE_STR, HEXA_BASE);
	write(1, "\n", 1);
}

void	print_chunks(t_segment *segment, bool is_large)
{
	size_t		chunk_size;
	t_chunk		*chunk;
	long		debut;

	chunk = (t_chunk*)LARGE_CHUNK_DATA(segment);
	while (chunk)
	{
		if (is_large == true || chunk->in_use)
		{
			debut = is_large ? (long)chunk : (long)CHUNK_DATA(chunk);
			chunk_size = is_large ? segment->u_u.seg_size : CHUNK_DATA_SIZE(chunk);
			ft_putnbr_base(debut, HEXA_BASE_STR, HEXA_BASE);
			write(1, " - ", 3);
			ft_putnbr_base(debut + chunk_size, HEXA_BASE_STR, HEXA_BASE);
			write(1, " : ", 3);
			ft_putnbr_base(chunk_size, DECI_BASE_STR, DECI_BASE);
			write(1, " octets\n", 8);
			if (is_large == false && chunk->next_size)
				chunk = GET_NEXT_CHUNK(chunk);
			else
				break;
		}
	}
}

void	print_small(t_segment *segment)
{
	t_chunk		*chunk;

	chunk = (t_chunk*)LARGE_CHUNK_DATA(segment);
	while (chunk)
	{
		if (chunk->in_use)
		{
			ft_putnbr_base((long)CHUNK_DATA(chunk), HEXA_BASE_STR, HEXA_BASE);
			write(1, " - ", 3);
			ft_putnbr_base((long)chunk + chunk->size, HEXA_BASE_STR, HEXA_BASE);
			write(1, " : ", 3);
			ft_putnbr_base(CHUNK_DATA_SIZE(chunk), DECI_BASE_STR, DECI_BASE);
			write(1, " octets\n", 8);
			if (chunk->next_size)
				chunk = GET_NEXT_CHUNK(chunk);
			else
				break;
		}
	}
}

void	print_large(t_segment *segment)
{
	t_chunk	*chunk;

	chunk = (t_chunk*)LARGE_CHUNK_DATA(segment);
	ft_putnbr_base((long)chunk, HEXA_BASE_STR, HEXA_BASE);
	write(1, " - ", 3);
	ft_putnbr_base((long)chunk + segment->u_u.seg_size, HEXA_BASE_STR, HEXA_BASE);
	write(1, " : ", 3);
	ft_putnbr_base(segment->u_u.seg_size, DECI_BASE_STR, DECI_BASE);
	write(1, " octets\n", 8);
}

void	show_alloc_mem()
{
	t_segment	*segment;
	short		i;

	i = -1;
	while (g_op[++i].segment_name)
	{
		write(1, "\n", 1);
		segment = *GET_APPROPRIATE_SEGMENT_TYPE(g_op[i].offset);
		while (segment)
		{
			print_segment_info(segment, g_op[i].segment_name);
			/*if (g_op[i].is_large)
			  print_large(segment);
			  else
			  print_small(segment);
			  */
			print_chunks(segment, g_op[i].is_large);
			segment = segment->next;
		}
	}
}
