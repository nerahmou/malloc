/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   defrag.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/08 16:30:24 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/02 17:55:47 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

bool	unused_region(t_region *region)
{
	t_chunk *chunk;


	chunk = (t_chunk*)((size_t)region + REG_HEAD_SIZE);
	while (chunk && chunk->in_use == false)
		chunk = chunk->next;
	return (chunk ==  0);
}

t_chunk	*merge_prev_freed(t_chunk *freed_chunk)
{
	t_chunk	*bin_elem;

	bin_elem = pop_specific(freed_chunk->prev);
	bin_elem->size += freed_chunk->size;
	bin_elem->next = freed_chunk->next;
	if (freed_chunk->next)
		freed_chunk->next->prev = bin_elem;
	return (bin_elem);
}

t_chunk	*merge_next_freed(t_chunk *freed_chunk)
{
	t_chunk	*bin_elem;

	bin_elem = pop_specific(freed_chunk->next);
	freed_chunk->size += bin_elem->size;
	freed_chunk->next = bin_elem->next;
	if (bin_elem->next)
		bin_elem->next->prev = freed_chunk;
	return (freed_chunk);
}

bool	defrag(t_region *region, t_chunk **chunk)
{
	t_chunk			*freed_chunk;
	unsigned short	bin_size_limit;

	freed_chunk = *chunk;
	if (freed_chunk->size <= TINY_MAX_SIZE)
		bin_size_limit = TINY_MAX_SIZE;
	else
		bin_size_limit = SMALL_MAX_SIZE;
	while (freed_chunk->prev && freed_chunk->prev->in_use == false
			&& freed_chunk->size + freed_chunk->prev->size <= bin_size_limit)
		freed_chunk = merge_prev_freed(freed_chunk);
	while (freed_chunk->next && freed_chunk->next->in_use == false
			&& freed_chunk->size + freed_chunk->next->size <= bin_size_limit)
		freed_chunk = merge_next_freed(freed_chunk);
	*chunk = freed_chunk;
	return (unused_region(region));
}
