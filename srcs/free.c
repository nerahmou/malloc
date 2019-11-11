/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/31 16:11:21 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/11 17:13:14 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	free_segment(t_segment **head, t_segment *to_munmap, t_op g_op)
{
	t_segment	*segment;
	size_t		size;

	segment = *head;
	size = g_op.is_large ? to_munmap->u_u.seg_size : g_op.segment_size;
	if (*head == to_munmap)
		*head = to_munmap->next;
	else
	{
		while (segment->next != to_munmap)
			segment = segment->next;
		segment->next = to_munmap->next;
	}
	MUNMAP(to_munmap, size);
	munmap(to_munmap, to_munmap->u_u.seg_size + SEG_HEAD_SIZE);
}

void	free_small(t_segment **head, t_segment *segment, void *addr, t_op g_op)
{
	t_chunk		*chunk;
	short		index;

	chunk = GET_CHUNK_HEADER(addr);
	if (chunk->in_use == true)
	{
		chunk->in_use = false;
		if (defrag(segment, chunk, g_op))
		{
			update_bins(segment);
			free_segment(head, segment, g_op);
		}
		else
		{
			index = BIN_INDEX(chunk->size);
			if (g_bins[index] != NULL)
				g_bins[index]->u_u.prev_free = chunk;
			chunk->next_free = g_bins[index];
			g_bins[index] = chunk;
		}
	}
}

void	free(void *addr)
{
	t_segment	**head;
	t_segment	*segment;
	short		i;

	if (addr != NULL)
	{
		i = 3;
		while (i--)
		{
			head = GET_APPROPRIATE_SEGMENT_TYPE(g_op[i].offset);
			segment = *head;
			while (segment)
			{
				if (CHUNK_IN_SEG((size_t)addr, (size_t)segment, g_op[i].segment_size))
				{
					if (g_op[i].is_large)
						FREE_LARGE(head, segment, g_op[i]);
					else
						free_small(head, segment, addr, g_op[i]);
					return;
				}
				segment = segment->next;
			}
		}
	}
}
