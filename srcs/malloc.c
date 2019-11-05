/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/05 18:19:16 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>


void	*new_segment(t_segment **head, t_op g_op, size_t len)
{
	t_segment	*new_seg;
	size_t		chunk_size;

	len = g_op.is_large ? len : g_op.segment_size;
	new_seg = MMAP(len);
	if (new_seg == MAP_FAILED)
		return (NULL);
	if (*head == NULL)
		*head = new_seg;
	new_seg->next = NULL;
	if (g_op.is_large)
		new_seg->u_u.seg_size = len - sizeof(t_segment);
	else
	{
		new_seg->u_u.last_chunk = MOVE_CHUNK_ADDR((t_chunk*)new_seg, SEG_HEAD_SIZE);
		new_seg->u_u.last_chunk->size = len - (sizeof(t_segment) + sizeof(t_chunk));
	}
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
	t_segment	*tmp_previous;

	tmp_previous = NULL;
	head = GET_APPROPRIATE_SEGMENT_TYPE(g_op.offset);
	segment = *head;
	while (segment)
	{
		if (!g_op.is_large && segment->u_u.last_chunk->size >= size)
			break;
		tmp_previous = segment;
		segment = segment->next;
	}
	if (segment == NULL)
	{	
		segment = new_segment(head, g_op, size);
		if (tmp_previous != NULL)
			tmp_previous->next = segment;
	}
	return (segment);
}

void	*place_chunk(t_op g_op, size_t size)
{
	t_segment	*segment;
	t_chunk		*new_chunk;
	t_chunk		*last_chunk;

	segment = get_segment(g_op, size);
	if (g_op.is_large || segment == NULL) //Pas besoin de configuger le chunk
		return (CHUNK_DATA(segment));
	last_chunk = segment->u_u.last_chunk;
	segment->u_u.last_chunk = MOVE_CHUNK_ADDR(segment->u_u.last_chunk, size);
	UPDATE_CHUNK_SIZE(segment->u_u.last_chunk, last_chunk->size - size);
	new_chunk = last_chunk;
	new_chunk->in_use = 1;
	UPDATE_CHUNK_SIZE(new_chunk, size);
	return (CHUNK_DATA(new_chunk));
}

void	*malloc(size_t size)
{
	void		*addr;
	short		i;

	addr = NULL;
	i = -1;
	while (g_op[++i].max_chunk_size)
	{
		if ((size / g_op[i].max_chunk_size) == 0)
		{
			size = GET_REQUIRED_SIZE(size);
			addr = place_chunk(g_op[i], size);
			break;
		}
	}
	return (addr);
}

