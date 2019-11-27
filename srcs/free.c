/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/26 13:54:48 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/27 17:31:35 by nerahmou    ###    #+. /#+    ###.fr     */
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
	if (MUNMAP(to_munmap, len) == MUNMAP_FAILED)
			ft_printf("munmap() failed: addr=[%p] | len=[%zu]\n", to_munmap, len);
}

void	free_small(t_region **head, t_region *region, void *ptr)
{
	t_chunk		*chunk = NULL;

	chunk = CHUNK_HEADER(ptr);
	chunk->in_use = false;
	if (defrag(region, &chunk))
	{
		update_bins(region);
		free_region(head, region);
	}
	else
		push((t_freed*)chunk);
}

t_region	*get_the_region(t_region *region, void *ptr, size_t region_size)
{
	while (region)
	{
		if (IN_REGION(ptr, region, region_size))
			return (region);
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
	//ft_printf("is_valid_ptr(): addr=[%p]\n", ptr);
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
		//ft_printf("\tis_valid_ptr(): = [%i]\n", i);
		while (region)
		{
			//if (i == 1)
			//	ft_printf("\t\tis_valid_ptr(): [%i] ptr=[%p]|region=[%p]|region_size=[%zu]\n",i, ptr, region, region_size);
			if (IN_REGION(ptr, region, region_size))
			{
		//		ft_printf("\t\tis_valid_ptr(): IN_REGION\n");
				chunk = FIRST_CHUNK(region);
				while (chunk->size)
				{
					if (&(chunk->data) == ptr)
						return (head);
					chunk = chunk->next;
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
	size_t	region_size;

	if (ptr != NULL)
	{
		head = is_valid_ptr(ptr);
		if (head)
		{
			if (head == &(g_heap.tiny_region))
				region_size = TINY_REGION_SIZE;
			else if (head == &(g_heap.small_region))
				region_size = SMALL_REGION_SIZE;
			else
				region_size = LARGE_REGION_SIZE;
			region = get_the_region(*head, ptr, region_size);
			if (head == &(g_heap.large_region))
				free_region(head, region);
			else if (head == &(g_heap.tiny_region) || head == &(g_heap.small_region))
				free_small(head, region, ptr);
		}
	}
}
