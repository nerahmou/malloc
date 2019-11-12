/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/12 11:19:52 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

/*
 * LARGE_SEG_SIZE = REQUIRED_SIZE + SEG_HEAD_SIZE;
 */
void	*new_segment(t_segment **head, t_op g_op, size_t len)
{
	t_segment	*new_seg;

	len = g_op.is_large ? len : g_op.segment_size;
	new_seg = MMAP(len);
	if (new_seg == MAP_FAILED)
		return (NULL);
	if (*head == NULL)
		*head = new_seg;
	new_seg->next = NULL;
	new_seg->u_u.seg_size = len - SEG_HEAD_SIZE; // Gere les deux cas vu que c'est uen union
	return (new_seg);
}

/*
 * get_segment():	Find and return a segment with enough space for the 
 *					size. If not found, call new_segment()
 */
t_segment	*get_segment(t_op g_op, size_t size)
{
	t_segment	*segment;
	t_segment	**head;
	t_segment	*previous_segment;

	previous_segment = NULL;
	head = GET_APPROPRIATE_SEGMENT_TYPE(g_op.offset);
	segment = *head;
	while (segment)
	{
		if (!g_op.is_large && segment->u_u.available_space >= size)
			break;
		previous_segment = segment;
		segment = segment->next;
	}
	if (segment == NULL)
	{	
		segment = new_segment(head, g_op, size);
		if (previous_segment != NULL)
			previous_segment->next = segment;
	}
	return (segment);
}

void	*place_chunk(t_op g_op, size_t size)
{
	t_segment	*segment;
	t_chunk		*new_chunk;
	t_chunk		*prev_chunk;

	segment = get_segment(g_op, size);
	if (g_op.is_large || segment == NULL) //Pas besoin de configuger le chunk
		return (LARGE_CHUNK_DATA(segment));
	new_chunk = SET_CHUNK_POS((long)segment, g_op.segment_size);
	prev_chunk = (t_chunk*)LARGE_CHUNK_DATA(segment);
	while (prev_chunk->next_size != 0)
		prev_chunk = GET_NEXT_CHUNK(prev_chunk);
	if (IS_FIRST_CHUNK(segment, g_op.segment_size) == false)
	{
		prev_chunk->next_size = size;
		new_chunk->prev = prev_chunk;
	}
	new_chunk->size = size; // Header compris
	new_chunk->next_size = 0;
	new_chunk->in_use = true;
	segment->u_u.available_space -= size;
	return (CHUNK_DATA(new_chunk));
}

void	*malloc(size_t size)
{
	void		*addr;
	short		i;

	addr = NULL;
	i = -1;
	if (size != 0)
	{
		while (g_op[++i].max_chunk_size)
		{
			if (GOOD_SEGMENT_TYPE(size, g_op[i]))
			{
				size = GET_REQUIRED_SIZE(size);
				addr = place_chunk(g_op[i], size);
				break;
			}
		}
	}
	return (addr);
}

