/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/31 16:11:21 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/08 17:40:57 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	free_small(t_segment **head, t_segment *segment, void *addr, t_op g_op)
{
	t_chunk		*chunk;
	short		index;

	chunk = GET_CHUNK_HEADER(addr);
	if (chunk->in_use == true)
	{
		chunk->in_use = false;
		defrag(segment, chunk, g_op);
		index = BIN_INDEX(chunk->size);
		if (g_bins[index] != NULL)
			g_bins[index]->u_u.prev_free = chunk;
		chunk->next_free = g_bins[index];
		g_bins[index] = chunk;
	}
}

void	free_large(t_segment **head, t_segment *to_munmap)
{
	t_segment *segment;

	segment = *head;
	if (*head == to_munmap)
		*head = to_munmap->next;
	else
	{
		while (segment->next != to_munmap)
			segment = segment->next;
		segment->next = to_munmap->next;
	}
	munmap(to_munmap, to_munmap->u_u.seg_size + SEG_HEAD_SIZE);
}

void	valid_addr(void *addr)
{
	t_segment	**head;
	t_segment	*segment;
	short		i;

	i = 3;
	while (i--)
	{
		head = GET_APPROPRIATE_SEGMENT_TYPE(g_op[i].offset);
		segment = *head;
		while (segment)
		{
			if (CHUNK_IN_SEG((long)addr, (long)segment, g_op[i].segment_size))
			{
				if (g_op[i].is_large)
					free_large(head, segment);
				else
					free_small(head, segment, addr, g_op[i]);
				return;
			}
			segment = segment->next;
		}
	}
}

void	free(void *addr)
{
	if (addr != NULL)
		valid_addr(addr);
}
