/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bins.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/11 15:12:54 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/11 15:54:25 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

/*
 * Parcours la liste d'une bin et retourne l'element correspondant au chunk
 * Sinon NULL
 */
t_chunk	*get_bin_elem(short bin_index, t_chunk *chunk, bool defrag)
{
	t_chunk	*bin_elem;

	bin_elem = g_bins[bin_index];
	if (defrag)
	{
		while(bin_elem && bin_elem != chunk)
			bin_elem = bin_elem->next_free;
	}
	return (bin_elem);
}

t_chunk	*pop_from_bin(t_chunk *chunk, bool defrag)
{
	t_chunk	*bin_elem;
	short	index;

	index = BIN_INDEX(chunk->size);
	bin_elem = get_bin_elem(index, chunk, defrag);
	if (bin_elem != NULL)
	{
		if (bin_elem->next_free)
			((t_chunk*)(bin_elem->next_free))->u_u.prev_free = bin_elem->u_u.prev_free;
		if (bin_elem->u_u.prev_free)
			((t_chunk*)(bin_elem->u_u.prev_free))->next_free = bin_elem->next_free;
		else
			g_bins[index] = bin_elem->next_free;
	}
	return (bin_elem);
}

/*
 * utilisé lors de la defragmentation afin de retirer tous les element presents
 * dans les bins appartenant au segment qui sera munmappé.
 */
void	update_bins(t_segment *segment)
{
	t_chunk *chunk;

	chunk = (t_chunk*)LARGE_CHUNK_DATA(segment);
	while (chunk->size && chunk->in_use == false)
	{
		pop_from_bin(chunk, true);
		chunk = NEXT_CHUNK(chunk);
	}
}
