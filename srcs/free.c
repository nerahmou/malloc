/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/26 13:54:48 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/02 18:51:01 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"
#include "malloc.h"

void	free_region(t_region **head, t_region *to_munmap)
{
	t_region	*region = NULL;
	size_t		len;

	region = *head;
	if (*head == to_munmap)
		*head = to_munmap->next;
	else
	{
		while (region->next != to_munmap)
			region = region->next;
		region->next = to_munmap->next;
	}
	if (head == &(g_heap.large_region))
		len = to_munmap->space + ((t_chunk*)((size_t)to_munmap + REG_HEAD_SIZE))->size;
	else if (head == &(g_heap.small_region))
		len = SMALL_REGION_SIZE;
	else
		len = TINY_REGION_SIZE;
	if (munmap(to_munmap, len) == MUNMAP_FAILED)
			ft_printf("munmap() failed: addr=[%p] | len=[%zu]\n", to_munmap, len);
}

void	free_small(t_region **head, t_region *region, void *ptr)
{
	t_chunk		*chunk = NULL;

	chunk = (t_chunk*)((size_t)ptr - CHUNK_HEAD_SIZE);
	chunk->in_use = false;
	if (defrag(region, &chunk))
	{
		update_bins(region);
		free_region(head, region);
	}
	else
		push(chunk);
	}

void	free(void *ptr)
{
	//t_region		**head = NULL;
	t_region		*region = NULL;
	size_t	region_size;

	if (ptr != NULL)
	{
		region = is_valid_ptr(ptr);
		if (region)
		{
			/*if (head == &(g_heap.tiny_region))
				region_size = TINY_REGION_SIZE;
			else if (head == &(g_heap.small_region))
				region_size = SMALL_REGION_SIZE;
			else
				region_size = LARGE_REGION_SIZE;
			region = get_region(*head, ptr, region_size);
			*/
			if (region->head == g_heap.large_region)
			{
			//	t_chunk *chunk = (t_chunk*)(ptr - 32);
				free_region(head, region);
			}
			/*else if (head == &(g_heap.tiny_region) || head == &(g_heap.small_region))
			{*/
			//	t_chunk *chunk = (t_chunk*)(ptr - 32);
			else
				free_small(head, region, ptr);
			//}
		}
	}
}
