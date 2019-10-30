/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/30 13:26:20 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>


void	*new_segment(t_segment **head, bool is_large, size_t len)
{
	t_segment	*new_seg;
	size_t		last_chunk_size;

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
//t_segment	*get_segment(t_segment **head, int seg_size, size_t malloc_size)
t_segment	*get_segment(t_op g_op, size_t malloc_size)
{
	t_segment	*segment;
	t_segment	**head;
	t_segment	*tmp_previous;
	size_t		segment_size;

	tmp_previous = NULL;
	head = GET_APPROPRIATE_SEGMENT_TYPE(g_op.offset);
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
		segment_size = g_op.is_large ? malloc_size : g_op.segment_size;
		segment = new_segment(head, g_op.is_large, segment_size);
		if (tmp_previous != NULL)
			tmp_previous->next = segment;
	}
	return (segment);
}

/*void	*large_chunk(t_segment *segment, int seg_size, size_t size)
{
	return NULL;
}
*/
//void	*small_chunk(t_segment **head, int seg_size, size_t malloc_size)
void	*place_chunk(t_op g_op, size_t malloc_size)
{
	t_segment	*segment;
	t_chunk		*new_chunk;
	t_chunk		*last_chunk;

	malloc_size = ROUND_NEXT_MULTIPLE(malloc_size);
	segment = get_segment(g_op, malloc_size);
	if (g_op.is_large || segment == NULL) //Pas besoin de chunk
		return (segment);
	last_chunk = segment->last_chunk;
	segment->last_chunk->size = segment->last_chunk->size - malloc_size;
	segment->last_chunk = MOVE_CHUNK_ADDR(last_chunk, malloc_size);
	new_chunk = last_chunk;
	new_chunk->size = malloc_size;
	new_chunk->in_use = 1;
	return (CHUNK_DATA(new_chunk));
}

void	*new_chunk(size_t size)
{
	void		*addr;
	short		i;

	addr = NULL;
	i = -1;
	while (g_op[++i].max_chunk_size)
	{
		if ((size / g_op[i].max_chunk_size) == 0)
		{
			addr = place_chunk(g_op[i], size);
			break;
		}
	}
	return (addr);
}

void	*malloc(size_t size)
{
	void	*addr;
	
	addr = new_chunk(size);
	return (addr);
}

