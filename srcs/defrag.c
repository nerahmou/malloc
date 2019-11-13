/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   defrag.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/08 16:30:24 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/13 13:47:22 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

bool	unused_region(t_region *region)
{
	t_chunk *chunk;

	chunk = GET_FIRST_CHUNK(region);
	while (chunk->size && chunk->in_use == false)
		chunk = NEXT_CHUNK(chunk);
	return (chunk->size == 0);
}

/*
 * Renvoie true si le region doit etre effacé
 */
bool	defrag(t_region *region, t_chunk **chunk, t_op g_op)
{
	t_chunk	*freed_chunk;
	t_chunk	*bin_elem;
	short	bin_size_limit;


	freed_chunk = *chunk;
	bin_size_limit = g_op.bin_size_limit;
	while (IS_PREV_FREE(freed_chunk, bin_size_limit))
	{
		bin_elem = pop_from_bin(freed_chunk->prev, true);
		bin_elem->size += freed_chunk->size;
		bin_elem->next_size = freed_chunk->next_size;
		if (freed_chunk->next_size)//Si on arrive pas dans la zone libre
			(NEXT_CHUNK(freed_chunk))->prev = bin_elem;
		freed_chunk = bin_elem;
	}
	while (IS_NEXT_FREE(freed_chunk, bin_size_limit))
	{
		bin_elem = pop_from_bin(NEXT_CHUNK(freed_chunk), true);
		freed_chunk->size += bin_elem->size;
		freed_chunk->next_size = bin_elem->next_size;
		if (freed_chunk->next_size)//Si on arrive pas dans la zone libre
			(NEXT_CHUNK(bin_elem))->prev = freed_chunk;
	}
	*chunk = freed_chunk;
	return (unused_region(region));
}
