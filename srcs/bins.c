/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bins.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/11 15:12:54 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/19 12:15:21 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

/*
 * utilisé lors de la defragmentation afin de retirer tous les element presents
 * dans les bins appartenant au region qui sera munmappé.
 */
void	update_bins(t_region *region)
{
	t_chunk *chunk;

	chunk = GET_FIRST_CHUNK(region);
	while (chunk->header.size && chunk->header.in_use == false)
	{
		pop(0, chunk);
		chunk = NEXT_CHUNK(chunk);
	}
}

/*
 * Parcours la liste d'une bin et retourne l'element correspondant au chunk
 * Sinon NULL
 */

void	push(t_chunk *chunk)
{
	short index;

	index = BIN_INDEX(chunk->header.size);
	if (g_bins[index] != NULL)
		g_bins[index]->u_u.prev_free = chunk;
	chunk->u_u.prev_free = NULL;
	chunk->next_free = g_bins[index];
	g_bins[index] = chunk;
}
/*
 * Le chunk est Null on souhaite retirer le premierelement de la bin
 * Le chunk est != Null si s'est un chunk precis que nous souhaitons retirer de la bin
 */
t_chunk	*pop(size_t size, t_chunk *chunk)
{
	t_chunk			*bin_elem;
	unsigned char	index;

	size = chunk == NULL ? size : chunk->header.size;
	index = BIN_INDEX(size);
	bin_elem = g_bins[index];
	if (chunk)
		while(bin_elem && bin_elem != chunk)
			bin_elem = bin_elem->next_free;
	if (bin_elem)
	{
		if (bin_elem->u_u.prev_free)
			GET_PREV_FREE(bin_elem)->next_free = bin_elem->next_free;
		if (bin_elem->next_free)
			GET_NEXT_FREE(bin_elem)->u_u.prev_free = bin_elem->u_u.prev_free;
		else
			g_bins[index] = bin_elem->next_free;
		ft_memset(CHUNK_DATA(bin_elem), 0, 16);
		if (chunk == NULL)
			bin_elem->header.in_use = true;
	}
	return (bin_elem);
}

t_chunk	*split_bin_elem(t_chunk *chunk, size_t bin_size, size_t size)
{
	t_chunk	*bin_elem;

	if (chunk == NULL || bin_size == size)
		return (chunk);
	bin_elem = CH_PTR((long)chunk + size);
	bin_elem->header.prev = chunk;
	bin_elem->header.size = bin_size - size;
	bin_elem->header.next_size = chunk->header.next_size;
	bin_elem->header.in_use = false;
	chunk->header.size = size;
	chunk->header.next_size = bin_elem->header.size;
	chunk->header.in_use = true;
	push(bin_elem);
	return (chunk);
}

t_chunk	*get_chunk_from_bin(t_chunk *chunk, size_t size, t_op g_op)
{
	t_chunk	*bin_elem;
	size_t	bin_size;
	size_t	bin_size_limit;

	if (g_op.is_large)
		return (NULL);
	bin_size = chunk == NULL ? size : chunk->header.size;
	bin_size_limit = g_op.max_chunk_size;
	bin_elem = pop(bin_size, chunk);
	if (bin_elem == NULL && chunk == NULL)
	{
		bin_size += (ALIGNEMENT * 2);
		while (bin_size <= bin_size_limit)
		{
			bin_elem = pop(bin_size, chunk);
			if (bin_elem != NULL)
				break ;
			bin_size += ALIGNEMENT;
		}
	}
	return (split_bin_elem(bin_elem, bin_size, size));
}


