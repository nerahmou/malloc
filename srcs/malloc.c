/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/18 14:25:52 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

/*
 * LARGE_size = REQUIRED_SIZE + SEG_HEAD_SIZE;
 */
void	*new_region(t_region **head, size_t len)
{
	t_region	*new_seg;

	new_seg = MMAP(len);
	if (new_seg == MAP_FAILED)
		return (NULL);
	if (*head == NULL)
		*head = new_seg;
	new_seg->next = NULL;
	new_seg->space = len; // Gere les deux cas vu que c'est uen union
	return (new_seg);
}

/*
 * get_region():	Find and return a region with enough space for the 
 *					size. If not found, call new_region()
 */
t_region	*get_region(t_op g_op, size_t *size, size_t *reg_size)
{
	t_region	**head;
	t_region	*region;
	t_region	*previous_region;

	previous_region = NULL;
	head = GET_APPROPRIATE_REGION_TYPE(g_op.offset);
	region = *head;
	*reg_size = g_op.is_large ? GET_REQUIRED_SIZE(*size, 4096) : g_op.reg_size;
	while (region)
	{
		if (AVAILABLE_SPACE(region, *size))
			return (region);
		previous_region = region;
		region = region->next;
	}
	if (region == NULL)
	{	
		region = new_region(head, *reg_size);
		if (previous_region != NULL)
			previous_region->next = region;
	}
	return (region);
}

t_chunk	*place_in_region(t_region *region, size_t region_size, size_t size)
{
	t_chunk		*prev_chunk;
	t_chunk		*new;

	new = SET_CHUNK_POS(region, region_size);
	if (IS_FIRST_CHUNK(region, new) == false)
	{
		prev_chunk = GET_FIRST_CHUNK(region);
		while (prev_chunk->header.next_size != 0)
			prev_chunk = GET_NEXT_CHUNK(prev_chunk);
		prev_chunk->header.next_size = size;
		(new)->header.prev = prev_chunk;
	}
	return (new);
}

void	*place_chunk(t_op g_op, size_t size)
{
	t_region	*region;
	t_chunk		*new_chunk;
	size_t		region_size;

	new_chunk = get_chunk_from_bin(size, g_op);
	if (new_chunk)
		return (CHUNK_DATA(new_chunk));
	region = get_region(g_op, &size, &region_size);
	if (region == NULL)
		return (NULL);
	new_chunk = place_in_region(region, region_size, size);
	new_chunk->header.size = size;
	new_chunk->header.next_size = 0;
	new_chunk->header.in_use = true;
	region->space -= size;
	return (CHUNK_DATA(new_chunk));
}

void	*malloc(size_t size)
{
	unsigned char	i;

	i = -1;
	if (size != 0)
	{
		size = GET_REQUIRED_SIZE(size, 16);
		while (g_op[++i].max_chunk_size)
			if (GOOD_region_TYPE(size, g_op[i]))
				return (place_chunk(g_op[i], size));
	}
	return (NULL);
}

