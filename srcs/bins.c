/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bins.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/11 15:12:54 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 17:56:30 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

/*
 * utilisé lors de la defragmentation afin de retirer tous les element presents
 * dans les bins appartenant au region qui sera munmappé.
 */
void	update_bins(t_region *region)
{
	t_chunk *chunk;

	chunk = FIRST_CHUNK(region);
	while (chunk->size/* && chunk->in_use == false*/)
	{
		pop(0, chunk);
		chunk = chunk->next;
	}
}

/*
 * Parcours la liste d'une bin et retourne l'element correspondant au chunk
 * Sinon NULL
 */

void	push(t_freed *chunk)
{
	short index;

	index = BIN_INDEX(chunk->size);
	if (g_bins[index] != NULL)
		g_bins[index]->prev_freed = chunk;
	chunk->prev_freed = NULL;
	chunk->next_freed = g_bins[index];
	g_bins[index] = chunk;
}
/*
 * Le chunk est Null on souhaite retirer le premierelement de la bin
 * Le chunk est != Null si s'est un chunk precis que nous souhaitons retirer de la bin
 */
t_chunk	*pop(size_t size, t_chunk *chunk)
{
	t_freed			*bin_elem;
	unsigned char	index;

	size = chunk == NULL ? size : chunk->size;
	index = BIN_INDEX(size);
	bin_elem = g_bins[index];
	if (chunk)
		while (bin_elem && bin_elem != (t_freed*)chunk)
			bin_elem = bin_elem->next_freed;
	if (bin_elem)
	{
		if (g_bins[index] == bin_elem)
			g_bins[index] = bin_elem->next_freed;//NEXT_FREE(bin_elem);
		if (PREV_FREED(bin_elem))
			bin_elem->next_freed->prev_freed = bin_elem->next_freed;//EXT_FREE(((t_chunk*)PREV_FREE(bin_elem))) = NEXT_FREE(bin_elem);
		if (NEXT_FREED(bin_elem))
			bin_elem->prev_freed->next_freed = bin_elem->prev_freed;//PREV_FREE(((t_chunk*)NEXT_FREE(bin_elem))) = PREV_FREE(bin_elem);
		ft_bzero(&(bin_elem->prev_freed), 16);
		if (chunk == NULL)
			bin_elem->in_use = true;
	}
	return ((t_chunk*)bin_elem);
}

t_chunk	*split_bin_elem(t_chunk *chunk, size_t bin_size, size_t size)
{
	t_chunk	*bin_elem;

	if (chunk == NULL || bin_size == size)
		return (chunk);
	bin_elem = (t_chunk*)((long)chunk + size);
	bin_elem->prev = chunk;
	bin_elem->size = bin_size - size;
	bin_elem->next = chunk->next;
	bin_elem->in_use = false;
	chunk->size = size;
	chunk->next = bin_elem;
	chunk->in_use = true;
	chunk->prev->next = chunk;
	push((t_freed*)bin_elem);
	return (chunk);
}

t_chunk	*get_chunk_from_bin(t_chunk *chunk, size_t size)
{
	t_chunk	*bin_elem;
	size_t	bin_size;
	size_t	bin_size_limit;

	if (size > SMALL_MAX_SIZE)
		return (NULL);
	bin_size = chunk == NULL ? size : chunk->size;
	if (bin_size <= TINY_MAX_SIZE)
		bin_size_limit = TINY_MAX_SIZE;
	else
		bin_size_limit = SMALL_MAX_SIZE;
	bin_elem = pop(bin_size, chunk);
	if (bin_elem == NULL && chunk == NULL)
	{
		bin_size += 32;
		while (bin_size <= bin_size_limit)
		{
			bin_elem = pop(bin_size, chunk);
			if (bin_elem != NULL)
				break ;
			bin_size += 16;
		}
	}
	return (/*split_bin_elem(bin_elem, bin_size, size)*/bin_elem);
}


