/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/29 14:32:35 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>


void	*new_segment(t_segment **head, size_t len)
{
	t_segment	*new_seg;

	new_seg = MMAP(len);
	if (new_seg == NULL)
		return (NULL);
	if (*head == NULL)
		*head = new_seg;
	new_seg->next = NULL;
	new_seg->last_chunk = (t_chunk*)new_seg + 1;
	new_seg->last_chunk->size = len - (sizeof(t_segment) + sizeof(t_chunk));
	return (new_seg);
}

/*
 * get_segment():	Find and return a segment with enough space for the 
 *					malloc_size. If not found, call new_segment()
 */
t_segment	*get_segment(t_segment **head, int seg_size, size_t malloc_size)
{
	t_segment	*segment;
	t_segment	*tmp_previous;

	tmp_previous = NULL;
	segment = *head;
	while (segment)
	{
		if (segment->last_chunk->size >= malloc_size)
			break;
		tmp_previous = segment;
		segment = segment->next;
	}
	if (segment == NULL)
	{	
		segment = new_segment(head, seg_size);
		if (tmp_previous != NULL)
			tmp_previous->next = segment;
	}
	return (segment);
}

void	*large_chunk(t_segment *segment, int seg_size, size_t size)
{
	return NULL;
}

void	*small_chunk(t_segment **head, int seg_size, size_t malloc_size)
{
	t_segment	*segment;
	t_chunk		*new_chunk;

	malloc_size = ROUND_NEXT_MULTIPLE(malloc_size);
	segment = get_segment(head, seg_size, malloc_size);
	if (segment == NULL)
		return (NULL);
	segment->last_chunk->size = segment->last_chunk->size - malloc_size;
	segment->last_chunk = segment->last_chunk + (malloc_size / ALIGNEMENT);
	new_chunk = segment->last_chunk - (malloc_size / ALIGNEMENT);
	new_chunk->size = malloc_size;
	new_chunk->in_use = 1;
	return (new_chunk + 1);
}

void	*new_chunk(size_t size)
{
	void		*addr;
	short		i;
	int			seg_size;
	t_segment	*segment;

	segment = NULL;
	addr = NULL;
	i = -1;
	while (g_op[++i].max_chunk_size)
	{
		if ((size / g_op[i].max_chunk_size) == 0)
		{
			seg_size = g_op[i].segment_size;
			segment = GET_APPROPRIATE_SEGMENT_TYPE(g_op[i].offset);
			addr = g_op[i].ptr_func(segment, seg_size, size);
			break;
		}
	}
	return (addr);
}

void	*malloc(size_t size)
{
	void	*addr;
	
	addr = NULL;
	addr = new_chunk(size);
	return (addr);
}

