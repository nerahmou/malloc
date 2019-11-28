/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bins.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/11 15:12:54 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/28 17:51:27 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

t_chunk	*split_bin_elem(t_chunk *chunk, size_t bin_size, size_t size)
{
	t_chunk	*bin_elem;

	if (chunk == NULL || bin_size == size)
		return (chunk);
	bin_elem = (t_chunk*)((size_t)chunk + size);
	bin_elem->prev = chunk;
	bin_elem->size = bin_size - size;
	bin_elem->next = chunk->next;
	bin_elem->in_use = false;
	chunk->size = size;
	chunk->next = bin_elem;
	chunk->in_use = true;
	push((t_chunk*)bin_elem);
	return (chunk);
}

void	*get_chunk_from_bin(size_t size)
{
	t_chunk	*bin_elem;
	size_t	bin_size;
	size_t	bin_size_limit;

	if (size > SMALL_MAX_SIZE)
		return (NULL);
	bin_size = size;
	if (bin_size <= TINY_MAX_SIZE)
		bin_size_limit = TINY_MAX_SIZE;
	else
		bin_size_limit = SMALL_MAX_SIZE;
	bin_elem = pop(size);
	if (bin_elem)
		return (&bin_elem->data);
	bin_size += 32;
	while (bin_size <= bin_size_limit)
	{
		bin_elem = pop(bin_size);
		if (bin_elem)
			return (&bin_elem->data);
		bin_size += 16;
	}
	return (split_bin_elem(bin_elem, bin_size, size));
}

/*
 * utilisé lors de la defragmentation afin de retirer tous les element presents
 * dans les bins appartenant au region qui sera munmappé.
 */
void	update_bins(t_region *region)
{
	t_chunk *chunk;

	chunk = (t_chunk*)((size_t)region + REG_HEAD_SIZE);
	while (chunk)
	{
		pop_specific((t_chunk*)chunk);
		chunk = chunk->next;
	}
}

/*
 * Parcours la liste d'une bin et retourne l'element correspondant au chunk
 * Sinon NULL
 */

void	push(t_chunk *chunk)
{
	short index;

	index = (chunk->size - CHUNK_HEAD_SIZE) / 16 - 1;
	chunk->data = g_bins[index];
	g_bins[index] = chunk;
}
/*
 * Le chunk est Null on souhaite retirer le premierelement de la bin
 * Le chunk est != Null si s'est un chunk precis que nous souhaitons retirer de la bin
 */

t_chunk	*pop_specific(t_chunk *chunk)
{
	t_chunk			*bin_elem;
	t_chunk			*prev_bin_elem;
	unsigned		index;

	index = (chunk->size - CHUNK_HEAD_SIZE) / 16 - 1;
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
	return ((t_chunk*)bin_elem);

}

t_chunk	*pop(size_t size)
{
	t_chunk			*bin_elem;
	t_chunk			*prev_bin_elem = NULL;
	unsigned short	index;

	index = (size - CHUNK_HEAD_SIZE) / 16 - 1;
	bin_elem = g_bins[index];
	if (bin_elem)
	{
		if (g_bins[index] == bin_elem)
			g_bins[index] = bin_elem->data;
		else
			prev_bin_elem->data = bin_elem->data;
		bin_elem->in_use = true;
		bin_elem->data=0;
	}
	return ((t_chunk*)bin_elem);
}


