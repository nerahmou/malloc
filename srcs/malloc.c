/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/28 17:27:00 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */


#include "malloc.h"
#include <stdio.h>


void	*new_segment(size_t len)
{
	t_segment	*new_seg;

	new_seg = MMAP(NULL, len);
	if (new_seg != NULL)
	{
		new_seg->next = NULL;
		new_seg->last_chunk = (t_chunk*)new_seg + 1;
		new_seg->last_chunk->size = len - sizeof(t_chunk);
	}
	return (new_seg);
}

void	*large_chunk(t_segment *segment, int segment_size, size_t size)
{
	return NULL;
}

void	*small_chunk(t_segment **head, int segment_size, size_t malloc_size)
{
	void		*data_addr;
	t_chunk		*new_chunk;
	t_segment	*segment;

	data_addr = NULL;
	segment = *head;
	malloc_size = GET_PADDED_CHUNK_SIZE(malloc_size);
	while (segment)
	{
		if (segment->last_chunk->size >= malloc_size)
			break;
		segment = segment->next;
	}
	if (segment == NULL && (segment = new_segment(segment_size)) == NULL)
		return (data_addr);
	segment->last_chunk->size = segment->last_chunk->size - malloc_size;
	segment->last_chunk = segment->last_chunk + (malloc_size / ALIGNEMENT);
	segment->last_chunk->in_use = segment->last_chunk->size == 0 ? 1 : 0;
	new_chunk = segment->last_chunk - (malloc_size / ALIGNEMENT);
	new_chunk->size = malloc_size;
	new_chunk->in_use = 1;
	data_addr = new_chunk + 1;
	return (data_addr);
}

void	*new_chunk(size_t size)
{
	void		*addr;
	short		i;
	int			max_chunk_size;
	int			segment_size;
	t_segment	*segment;

	segment = NULL;
	addr = NULL;
	i = 0;
	while (g_op[i].max_chunk_size)
	{
		max_chunk_size = g_op[i].max_chunk_size;
		if ((size / max_chunk_size) == 0)
		{
			segment_size = g_op[i].segment_size;
			segment = GET_SEGMENT_TYPE(g_op[i].offset);
			addr = g_op[i].ptr_func(segment, segment_size, size);
			break;
		}
		i++;
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

