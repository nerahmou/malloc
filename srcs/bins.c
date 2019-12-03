/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bins.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/11 15:12:54 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/03 18:42:47 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

t_chunk	*split_bin_elem(t_chunk *chunk, size_t size)
{
	t_chunk	*bin_elem;

	if (chunk->size > TINY_MAX_SIZE && chunk->size - size <= TINY_MAX_SIZE)
		return (chunk);
	bin_elem = (t_chunk*)((size_t)chunk + size);
	bin_elem->prev = chunk;
	bin_elem->size = chunk->size - size;
	bin_elem->next = chunk->next;
	bin_elem->in_use = false;
	chunk->size = size;
	chunk->next = bin_elem;
	chunk->in_use = true;
	push(bin_elem);
	return (chunk);
}

void	update_bins(t_region *region)
{
	t_chunk *chunk;

	chunk = (t_chunk*)((size_t)region + REG_HEAD_SIZE);
	while (chunk)
	{
		pop_specific(chunk);
		chunk = chunk->next;
	}
}

void	push(t_chunk *chunk)
{
	unsigned short	index;

	index = ((chunk->size - CHUNK_HEAD_SIZE) >> 4) - 1;
	chunk->data = g_bins[index];
	g_bins[index] = chunk;
}

t_chunk	*pop_specific(t_chunk *chunk)
{
	t_chunk			*bin_elem;
	t_chunk			*prev_bin_elem;
	unsigned short	index;

	index = ((chunk->size - CHUNK_HEAD_SIZE) >> 4) - 1;
	bin_elem = g_bins[index];
	while (bin_elem && bin_elem != chunk)
	{
		prev_bin_elem = bin_elem;
		bin_elem = bin_elem->data;
	}
	if (bin_elem)
	{
		if (g_bins[index] == bin_elem)
			g_bins[index] = bin_elem->data;
		else
			prev_bin_elem->data = bin_elem->data;
		bin_elem->data = 0;
	}
	return (bin_elem);
}

t_chunk	*pop(size_t size)
{
	unsigned short	index;
	t_chunk			*bin_elem;

	index = ((size - CHUNK_HEAD_SIZE) >> 4) - 1;
	bin_elem = g_bins[index];
	if (bin_elem)
	{
		g_bins[index] = bin_elem->data;
		bin_elem->in_use = true;
		bin_elem->data = 0;
	}
	return (bin_elem);
}
