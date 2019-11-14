/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/31 16:11:21 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/13 16:19:57 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	free_region(t_region **head, t_region *to_munmap, t_op g_op)
{
	t_region	*region;
	size_t		size;

	region = *head;
	size = g_op.is_large ? to_munmap->u_u.size : g_op.region_size;
	if (*head == to_munmap)
		*head = to_munmap->next;
	else
	{
		while (region->next != to_munmap)
			region = region->next;
		region->next = to_munmap->next;
	}
	MUNMAP(to_munmap, size);
}

void	free_small(t_region **head, t_region *region, void *addr, t_op g_op)
{
	t_chunk		*chunk;
	short		index;

	chunk = GET_CHUNK_HEADER(addr);
	if (chunk->in_use == true)
	{
		chunk->in_use = false;
		if (defrag(region, &chunk, g_op))
		{
			update_bins(region);
			free_region(head, region, g_op);
		}
		else
			push_bin(chunk);
	}
}

void	free(void *addr)
{
	t_region	**head;
	t_region	*region;
	short		i;

	if (addr != NULL)
	{
		i = 3;
		while (i--)
		{
			head = GET_APPROPRIATE_region_TYPE(g_op[i].offset);
			region = *head;
			while (region)
			{
				if (CHUNK_IN_SEG((size_t)addr, (size_t)region, g_op[i].region_size))
				{
					if (g_op[i].is_large)
						FREE_LARGE(head, region, g_op[i]);
					else
						free_small(head, region, addr, g_op[i]);
					return;
				}
				region = region->next;
			}
		}
	}
}
