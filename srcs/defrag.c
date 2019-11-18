/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   defrag.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/08 16:30:24 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/18 14:25:54 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

bool	unused_region(t_region *region)
{
	t_chunk *chunk;

	chunk = GET_FIRST_CHUNK(region);
	while (chunk->header.size && chunk->header.in_use == false)
		chunk = NEXT_CHUNK(chunk);
	return (chunk->header.size == 0);
}

t_chunk	*merge_previous(t_chunk *freed_chunk)
{
	t_chunk	*bin_elem;

	bin_elem = pop(0, freed_chunk->header.prev);
	bin_elem->header.size += freed_chunk->header.size;
	bin_elem->header.next_size = freed_chunk->header.next_size;
	if (freed_chunk->header.next_size)//Si on arrive pas dans la zone libre
		(NEXT_CHUNK(freed_chunk))->header.prev = bin_elem;
	return (bin_elem);
}

t_chunk	*merge_next(t_chunk *freed_chunk)
{
	t_chunk	*bin_elem;

	bin_elem = pop(0, NEXT_CHUNK(freed_chunk));
	freed_chunk->header.size += bin_elem->header.size;
	freed_chunk->header.next_size = bin_elem->header.next_size;
	if (freed_chunk->header.next_size)//Si on arrive pas dans la zone libre
		(NEXT_CHUNK(bin_elem))->header.prev = freed_chunk;
	return (freed_chunk);
}

bool	defrag(t_region *region, t_chunk **chunk, t_op g_op)
{
	t_chunk	*freed_chunk;
	size_t	bin_size_limit;


	freed_chunk = *chunk;
	bin_size_limit = g_op.max_chunk_size;
	while (IS_PREV_FREE(freed_chunk, bin_size_limit))
		freed_chunk = merge_previous(freed_chunk);
	while (IS_NEXT_FREE(freed_chunk, bin_size_limit))
		freed_chunk = merge_next(freed_chunk);
	*chunk = freed_chunk;
	return (unused_region(region));
}
