/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/31 16:11:21 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/07 13:56:44 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

/*
 * Types are long to make addresses calcul easier
 */

void	free_small(t_segment **head, t_segment *segment, void *addr, t_op g_op)
{
	t_chunk		*chunk;
	short		index;

	chunk = GET_CHUNK_HEADER(addr);
	if (chunk->in_use == true)
	{
		chunk->in_use = false;
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

bool	chunk_in_segment(long addr, long segment, long segment_size)
{
	return (addr > segment && addr < segment + segment_size);
}

void	valid_addr(void *addr)
{
	t_segment	**head;
	t_segment	*segment;
	long		segment_size;
	short		i;

	i = 3;
	while (i--)
	{
		head = GET_APPROPRIATE_SEGMENT_TYPE(g_op[i].offset);
		segment = *head;
		segment_size = g_op[i].is_large ? LARGE_SEG_SIZE : g_op[i].segment_size;
		while (segment)
		{
			if (chunk_in_segment((long)addr, (long)segment, segment_size))
			{
				if (g_op[i].is_large)
					free_large(head, segment);
				free_small(head, segment, addr, g_op[i]);
				return ;
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
