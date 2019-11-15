/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/15 16:32:23 by nerahmou    ###    #+. /#+    ###.fr     */
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

	len = len + SEG_HEAD_SIZE;
	new_seg = MMAP(len);
	if (new_seg == MAP_FAILED)
		return (NULL);
	if (*head == NULL)
		*head = new_seg;
	new_seg->next = NULL;
	new_seg->size = len; // Gere les deux cas vu que c'est uen union
	return (new_seg);
}

/*
 * get_region():	Find and return a region with enough space for the 
 *					size. If not found, call new_region()
 */
t_region	*get_region(t_op g_op, size_t size)
{
	t_region	*region;
	t_region	**head;
	t_region	*previous_region;

	previous_region = NULL;
	head = GET_APPROPRIATE_region_TYPE(g_op.offset);
	region = *head;
	while (region)
	{
		if (AVAILABLE_SPACE(region) >= size)
			return (region);
		previous_region = region;
		region = region->next;
	}
	if (region == NULL)
	{	
		size = g_op.is_large ? size : g_op.region_size;
		region = new_region(head, size);
		if (previous_region != NULL)
			previous_region->next = region;
	}
	return (region);
}



void	*place_chunk(t_op g_op, size_t size)
{
	t_region	*region;
	t_chunk		*new_chunk;
	t_chunk		*prev_chunk;
	size_t		region_size;

	new_chunk = get_chunk_from_bin(size, g_op);
	if (new_chunk)
		return (CHUNK_DATA(new_chunk));
	region = get_region(g_op, size);
	if (region == NULL)
		return (NULL);
	region_size = g_op.is_large ? size : g_op.region_size;
	new_chunk = SET_CHUNK_POS(region, region_size);
	prev_chunk = GET_FIRST_CHUNK(region);
	if (IS_FIRST_CHUNK(region, new_chunk) == false)
	{
		while (prev_chunk->next_size != 0)
			prev_chunk = GET_NEXT_CHUNK(prev_chunk);
		prev_chunk->next_size = size;
		new_chunk->prev = prev_chunk;
	}
	new_chunk->size = size;
	new_chunk->next_size = 0;
	new_chunk->in_use = true;
	region->size -= size;
	return (CHUNK_DATA(new_chunk));
}

void	*malloc(size_t size)
{
	unsigned char	i;

	i = 0;
	if (size != 0)
	{
		size = GET_REQUIRED_SIZE(size);
		while (g_op[i].max_chunk_size)
		{
			if (GOOD_region_TYPE(size, g_op[i]))
				return (place_chunk(g_op[i], size));
			i++;
		}
	}
	return (NULL);
}

