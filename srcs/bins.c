/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bins.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/11 15:12:54 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/13 14:09:15 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

/*
 * Parcours la liste d'une bin et retourne l'element correspondant au chunk
 * Sinon NULL
 */

void	push_bin(t_chunk *chunk)
{
	short index;

	index = BIN_INDEX(chunk->size);
	if (g_bins[index] != NULL)
		g_bins[index]->u_u.prev_free = chunk;
	chunk->next_free = g_bins[index];
	g_bins[index] = chunk;
}
t_chunk	*get_bin_elem(short bin_index, t_chunk *chunk, bool defrag)
{
	t_chunk	*bin_elem;

/*	bin_elem = g_bins[bin_index];
	if (defrag)
	{
		while(bin_elem && bin_elem != chunk)
			bin_elem = bin_elem->next_free;
	}
*/	return (bin_elem);
}

t_chunk	*pop_from_bin(t_chunk *chunk, bool defrag)
{
	t_chunk	*bin_elem;
	short	index;

	index = BIN_INDEX(chunk->size);
	//bin_elem = get_bin_elem(index, chunk, defrag);
	bin_elem = g_bins[index];
	if (defrag)
	{
		while(bin_elem && bin_elem != chunk)
			bin_elem = bin_elem->next_free;
	}
	if (bin_elem != NULL)
	{
		if (bin_elem->next_free)
			GET_NEXT_FREE(bin_elem)->u_u.prev_free = bin_elem->u_u.prev_free;
		if (bin_elem->u_u.prev_free)
			GET_PREV_FREE(bin_elem)->next_free = bin_elem->next_free;
		else
			g_bins[index] = bin_elem->next_free;
		bin_elem->u_u.prev_free = NULL;
		bin_elem->next_free = NULL;
	}
	return (bin_elem);
}

t_chunk	*get_chunk_from_bin(size_t size, t_op g_op)
{
	t_chunk	*chunk;
	t_chunk	*new_bin_elem;
	size_t	bin_size;
	size_t	bin_size_limit;

	bin_size = size;
	bin_size_limit = g_op.max_chunk_size;
	chunk = pop_from_bin(g_bins[BIN_INDEX(bin_size)], false);
	if (chunk == NULL)
	{
		bin_size += (ALIGNEMENT * 2);
		while (bin_size < bin_size_limit)
		{
			chunk = pop_from_bin(g_bins[BIN_INDEX(bin_size)], false);
			if (chunk != NULL)
			{
				new_bin_elem = CH_PTR((long)chunk + size);
				new_bin_elem->prev = chunk;
				new_bin_elem->size = bin_size - size;
				new_bin_elem->next_size = chunk->next_size;
				new_bin_elem->in_use = false; // voir si utile
				chunk->size = size;
				chunk->next_size = new_bin_elem->size;
				chunk->in_use = true;
				push_bin(new_bin_elem);
				break;
			}
			bin_size += ALIGNEMENT;
		}
	}
	return (chunk);
}

/*
 * utilisé lors de la defragmentation afin de retirer tous les element presents
 * dans les bins appartenant au region qui sera munmappé.
 */
void	update_bins(t_region *region)
{
	t_chunk *chunk;

	chunk = GET_FIRST_CHUNK(region);
	while (chunk->size && chunk->in_use == false)
	{
		pop_from_bin(chunk, true);
		chunk = NEXT_CHUNK(chunk);
	}
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
