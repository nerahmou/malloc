/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/26 13:54:48 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/02 19:33:00 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"
#include "malloc.h"

void	free_region(t_region *to_munmap)
{
	t_region	*region = NULL;

	region = *to_munmap->head;
	if (*to_munmap->head == to_munmap)
		*to_munmap->head = to_munmap->next;
	else
	{
		while (region->next != to_munmap)
			region = region->next;
		region->next = to_munmap->next;
	}
	if (munmap(to_munmap, to_munmap->size) == MUNMAP_FAILED)
			ft_printf("munmap() failed: addr=[%p]", to_munmap);
}

void	free_small(t_region *region, void *ptr)
{
	t_chunk		*chunk = NULL;

	chunk = (t_chunk*)((size_t)ptr - CHUNK_HEAD_SIZE);
	chunk->in_use = false;
	if (defrag(region, &chunk))
	{
		update_bins(region);
		free_region(region);
	}
	else
		push(chunk);
	}

void	free(void *ptr)
{
	t_region	*region;

	if (ptr != NULL)
	{
		region = is_valid_ptr(ptr);
		if (region)
		{
			if (region->head == &(g_heap.large_region))
				free_region(region);
			else
				free_small(region, ptr);
		}
	}
}
