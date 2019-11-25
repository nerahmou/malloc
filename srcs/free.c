/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/31 16:11:21 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 14:15:48 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

void	free_region(t_region **head, t_region *to_munmap)
{
	t_region	*region = NULL;
	size_t		size;

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
		size = to_munmap->space + ((t_chunk*)((size_t)to_munmap + REG_HEAD_SIZE))->size;
	else if (head == &(g_heap.small_region))
		size = SMALL_REGION_SIZE;
	else
		size = TINY_REGION_SIZE;
	if (MUNMAP(to_munmap, size) == MUNMAP_FAILED)
			;////ft_printf("munmap() failed: addr=[%p] | len=[%zu]\n", to_munmap, size);
}

void	free_small(t_region **head, t_region *region, void *ptr)
{
	t_chunk		*chunk = NULL;

	chunk = CHUNK_HEADER(ptr);
	chunk->in_use = false;
//	if (chunk->in_use == true)
//	{
		/*if (defrag(region, &chunk, g_op))
		{
			update_bins(region);
			free_region(head, region, g_op);
		}
		else
			push(chunk);*/
//	}
}

t_region	*get_the_region(t_region *region, void *ptr)
{
	while (region)
	{
		if (IN_REGION(ptr, region, region->space))
			break;
		region = region->next;
	}
	return (region);
}

t_region	**is_valid_ptr(void *ptr)
{
	unsigned char	i;
	t_region		**head = NULL;
	t_region		*region = NULL;
	t_chunk			*chunk = NULL;
	size_t			region_size;

	i = -1;
	while (++i < 3)
	{
		head = (&g_heap.tiny_region + i);
		if (i == 2)
			region_size = LARGE_REGION_SIZE;
		else if (i == 1)
			region_size = SMALL_REGION_SIZE;
		else
			region_size = TINY_REGION_SIZE;
		region = *head;
		while (region)
		{
			if (IN_REGION(ptr, region, region_size))
			{
				chunk = FIRST_CHUNK(region);
				while (chunk->size)
				{
					if (&(chunk->data) == ptr)
						return (head);
					chunk = NEXT_CHUNK(chunk);
				}
			}
			region = region->next;
		}
	}
	return (NULL);
}

void	free(void *ptr)
{
	t_region		**head = NULL;
	t_region		*region = NULL;
	unsigned char	i;
	
	if (ptr != NULL)
	{
		head = is_valid_ptr(ptr);
		if (head)
		{
			region = get_the_region(*head, ptr);
			if (head == &(g_heap.large_region))
				free_region(head, region);
			else if (head == &(g_heap.tiny_region) || head == &(g_heap.small_region))
				free_small(head, region, ptr);
			return ;
		}
	}
}
