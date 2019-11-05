/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/31 16:11:21 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/05 17:18:41 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

/*
 * Types are long to make addresses calcul easier
 */

//void	*munmap_region()

void	*free_large(t_segment **head, t_segment *to_munmap)
{
	t_segment *segment;
	t_segment *next;

	segment = *head;
	if (*head == to_munmap)
		*head = to_munmap->next;
	else
	{
		while (segment->next != to_munmap)
			segment = segment->next;
		segment->next = to_munmap->next;
	}
	munmap(to_munmap, 10);
	return (NULL);
}

bool	chunk_in_segment(long addr, long segment, long segment_size)
{
	return (addr > segment && addr < segment + segment_size);
}

t_segment	*valid_addr(void *addr)
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
		while(segment)
		{
			if (chunk_in_segment((long)addr, (long)segment, segment_size))
			{
				if (g_op[i].is_large)
					return (free_large(head, segment));
				return (segment);
			}
			segment = segment->next;
		}
	}
	return (NULL);
}

void	free(void *addr)
{
	t_segment	*segment;
	t_chunk		*chunk;
	short		index;

	if (addr != NULL)
	{
		chunk = (t_chunk *)addr - 1;
		segment = valid_addr(addr);
		if (segment && chunk->in_use == true)
		{
			index = BIN_INDEX(chunk->size);
			chunk->in_use = false;
			addr = g_bins[index];
			g_bins[index] = chunk + 1;
		}
	}
}
