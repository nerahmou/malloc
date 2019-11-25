/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   defrag.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/08 16:30:24 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 17:03:38 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

bool	unused_region(t_region *region)
{
	t_chunk *chunk;

	chunk = FIRST_CHUNK(region);
	while (chunk->size && chunk->in_use == false)
		chunk = chunk->next;
	return (chunk->size == 0);
}

t_chunk	*merge_prev_free(t_chunk *freed_chunk)
{
	t_chunk	*bin_elem;

	bin_elem = pop(0, freed_chunk->prev);
	bin_elem->size += freed_chunk->size;
	bin_elem->next = freed_chunk->next;
	if (freed_chunk->next)
		freed_chunk->next->prev = bin_elem;
	return (bin_elem);
}

t_chunk	*merge_next_free(t_chunk *freed_chunk)
{
	t_chunk	*bin_elem;

	bin_elem = pop(0, freed_chunk->next);
	freed_chunk->size += bin_elem->size;
	freed_chunk->next = bin_elem->next;
	if (freed_chunk->next)
		freed_chunk->next->prev = freed_chunk;
	return (freed_chunk);
}

bool	defrag(t_region *region, t_chunk **chunk)
{
	t_chunk		*freed_chunk;
	unsigned	bin_size_limit;

	freed_chunk = *chunk;
	if (freed_chunk->size <= TINY_MAX_SIZE)
		bin_size_limit = TINY_MAX_SIZE;
	else
		bin_size_limit = SMALL_MAX_SIZE;
	while (IS_PREV_FREE(freed_chunk) && INF_MAX_BIN(freed_chunk, PREV_CHUNK(freed_chunk), bin_size_limit))
		freed_chunk = merge_prev_free(freed_chunk);
	while (IS_NEXT_FREE(freed_chunk) && INF_MAX_BIN(freed_chunk, (NEXT_CHUNK(freed_chunk)), bin_size_limit))
		freed_chunk = merge_next_free(freed_chunk);
	*chunk = freed_chunk;
	return (unused_region(region));
}
