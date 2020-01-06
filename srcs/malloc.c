/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/06 17:09:55 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#include "free.h"

t_chunk	*place_in_region(t_region *reg, size_t size)
{
	register t_chunk	*prev_chunk;
	t_chunk				*new;
	size_t				reg_size;

	if (size > SMALL_MAX_SIZE)
		reg_size = reg->space;
	else
		reg_size = reg->size;
	new = (t_chunk*)((size_t)reg + sizeof(t_region) + (reg_size - reg->space));
	if (!(new == (t_chunk*)((size_t)reg + sizeof(t_region))))
	{
		prev_chunk = (t_chunk*)((size_t)reg + sizeof(t_region));
		while (prev_chunk->next != NULL)
			prev_chunk = prev_chunk->next;
		prev_chunk->next = new;
		new->prev = prev_chunk;
	}
	else
		new->prev = NULL;
	new->size = size;
	new->next = NULL;
	new->in_use = true;
	reg->space -= size;
	return (new);
}

void	*place_chunk(size_t size)
{
	t_chunk		*new_chunk;
	t_region	*region;

	region = get_region_to_place_chunk(size);
	if (region == NULL)
		return (NULL);
	new_chunk = place_in_region(region, size);
	return (&(new_chunk->data));
}

void	*get_chunk_from_bin(size_t size)
{
	register size_t	bin_size;
	register size_t	bin_size_limit;
	t_chunk			*bin_elem;

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
	bin_size += MIN_BIN_SIZE;
	while (bin_size <= bin_size_limit)
	{
		bin_elem = pop(bin_size);
		if (bin_elem)
			return (&bin_elem->data);
		bin_size += ALIGNMENT;
	}
	return (NULL);
}

void	*malloc(size_t size)
{
	void *addr;

	addr = NULL;
	if (size == 0)
		++size;
	size = required_size(size, sizeof(t_chunk) - 8, ALIGNMENT);
	if (size != 0)
	{
		addr = get_chunk_from_bin(size);
		if (addr == NULL)
			addr = place_chunk(size);
	}
	return (addr);
}
