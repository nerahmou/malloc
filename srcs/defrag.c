/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   defrag.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/08 16:30:24 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/08 17:40:50 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_chunk	*get_bin_elem(short bin_index, t_chunk *chunk)
{
	t_chunk	*bin_elem;

	bin_elem = g_bins[bin_index];
	while(bin_elem && bin_elem != chunk)
		bin_elem = bin_elem->next_free;
	return (bin_elem);
}



t_chunk	*pop_from_bin(t_chunk *chunk)
{
	t_chunk	*bin_elem;
	short	index;

	index = BIN_INDEX(chunk->size);
	bin_elem = get_bin_elem(index, chunk);
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


void	*defrag(t_segment *segment, t_chunk *chunk, t_op g_op)
{
	t_chunk *tmp;
	short	bin_max_size;
	
	tmp = chunk;
	bin_max_size = g_op.max_chunk_size;
	while (IS_PREV_FREE(tmp, bin_max_size))
	{
		pop_from_bin(tmp->prev);
		tmp->prev->size += tmp->size;
		tmp->prev->next_size = tmp->next_size;
		tmp = tmp->prev;
	}

	return (NULL);
}
