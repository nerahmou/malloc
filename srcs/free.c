/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/31 16:11:21 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/18 14:26:09 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	free_region(t_region **head, t_region *to_munmap, t_op g_op)
{
	t_region	*region;
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
	size = g_op.is_large ?
		to_munmap->space + (GET_FIRST_CHUNK(to_munmap))->header.size :
		g_op.reg_size;
	MUNMAP(to_munmap, size);
}

void	free_small(t_region **head, t_region *region, void *addr, t_op g_op)
{
	t_chunk		*chunk;

	chunk = GET_CHUNK_HEADER(addr);
	if (chunk->header.in_use == true)
	{
		chunk->header.in_use = false;
		if (defrag(region, &chunk, g_op))
		{
			update_bins(region);
			free_region(head, region, g_op);
		}
		else
			push(chunk);
	}
}

t_region	*get_the_region(t_region *region, void *ptr, unsigned char i)
{
	while (region)
	{
		if (CHUNK_IN_SEG((size_t)ptr, (size_t)region, g_op[i].reg_size))
			break;
		region = region->next;
	}
	return (region);
}

unsigned char	is_valid_ptr(void *ptr)
{
	unsigned char	i;
	t_region	*region;

	i = -1;
	while (++i < 3)
	{
		region = *GET_APPROPRIATE_REGION_TYPE(g_op[i].offset);
		while (region)
		{
			if (CHUNK_IN_SEG((size_t)ptr, (size_t)region, g_op[i].reg_size))
				return (i);
			region = region->next;
		}
	}
	return (i);
}

void	free(void *ptr)
{
	t_region		**head;
	t_region		*region;
	unsigned char	i;

	if (ptr != NULL)
	{
		i = is_valid_ptr(ptr);
		if (g_op[i].region_name != NULL)
		{
			head = GET_APPROPRIATE_REGION_TYPE(g_op[i].offset);
			region = get_the_region(*head, ptr, i);
			if (g_op[i].is_large)
				FREE_LARGE(head, region, g_op[i]);
			else
				free_small(head, region, ptr, g_op[i]);
		}
	}
}
