/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   defrag.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/08 16:30:24 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/05 13:21:22 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

bool	unused_region(t_region *region)
{
	register t_chunk *chunk;

	chunk = (t_chunk*)((size_t)region + REG_HEAD_SIZE);
	while (chunk && chunk->in_use == false)
		chunk = chunk->next;
	return (chunk == 0);
}

t_chunk	*merge_prev_freed(t_chunk *freed)
{
	t_chunk	*bin_elem;

	bin_elem = pop_specific(freed->prev);
	bin_elem->size += freed->size;
	bin_elem->next = freed->next;
	if (freed->next)
		freed->next->prev = bin_elem;
	return (bin_elem);
}

t_chunk	*merge_next_freed(t_chunk *freed)
{
	t_chunk	*bin_elem;

	bin_elem = pop_specific(freed->next);
	freed->size += bin_elem->size;
	freed->next = bin_elem->next;
	if (bin_elem->next)
		bin_elem->next->prev = freed;
	return (freed);
}

bool	defrag(t_region *region, t_chunk **chunk)
{
	register t_chunk	*freed;

	freed = *chunk;
	while (freed->prev && freed->prev->in_use == false
			&& freed->size + freed->prev->size <= region->max_chunk_size)
		freed = merge_prev_freed(freed);
	while (freed->next && freed->next->in_use == false
			&& freed->size + freed->next->size <= region->max_chunk_size)
		freed = merge_next_freed(freed);
	*chunk = freed;
	return (unused_region(region));
}
