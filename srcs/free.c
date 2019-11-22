/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/31 16:11:21 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/22 17:28:24 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

void	free_region(t_region **head, t_region *to_munmap, t_op g_op)
{
	t_region	*region;
	size_t		size;

	return;
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
		to_munmap->space + (FIRST_CHUNK(to_munmap))->header.size :
		g_op.reg_size;
	////ft_printf("MUNMAP : space=[%zu] first_chunk=[%zu] addr=[%p] | len=[%zu]\n", to_munmap->space, (FIRST_CHUNK(to_munmap))->header.size , to_munmap, size);
	if (MUNMAP(to_munmap, size) == MUNMAP_FAILED)
			;////ft_printf("munmap() failed: addr=[%p] | len=[%zu]\n", to_munmap, size);
}

void	free_small(t_region **head, t_region *region, void *ptr, t_op g_op)
{
	t_chunk		*chunk;

	chunk = CHUNK_HEADER(ptr);
	if (chunk->header.in_use == true)
	{
		chunk->header.in_use = false;
		/*if (defrag(region, &chunk, g_op))
		{
			update_bins(region);
			free_region(head, region, g_op);
		}
		else
			push(chunk);*/
	}
}

t_region	*get_the_region(t_region *region, void *ptr, unsigned char i)
{
	while (region)
	{
		if (IN_REGION((size_t)ptr, (size_t)region, g_op[i].reg_size))
			break;
		region = region->next;
	}
	return (region);
}

unsigned char	is_valid_ptr(void *ptr)
{
	unsigned char	i;
	t_region		*region;
	t_chunk			*chunk;

	i = -1;
	while (++i < 3)
	{
		region = *APPROPRIATE_REGION_TYPE(g_op[i].offset);
		while (region)
		{
			if (IN_REGION((size_t)ptr, (size_t)region, g_op[i].reg_size))
			{
				//ft_printf("//[FREE TROUVE]\n");
				return (i);
			}
			region = region->next;
		}
	}
	//ft_printf("//[FREE PAS TROUVE]\n");
	return (i);
}

void	free(void *ptr)
{
	t_region		**head;
	t_region		*region;
	unsigned char	i;

	//ft_printf("free(%p);", ptr);
	if (ptr != NULL)
	{
		i = is_valid_ptr(ptr);
		if (g_op[i].region_name != NULL)
		{
			head = APPROPRIATE_REGION_TYPE(g_op[i].offset);
			region = get_the_region(*head, ptr, i);
			if (g_op[i].is_large)
				FREE_LARGE(head, region, g_op[i]);
			else
				free_small(head, region, ptr, g_op[i]);
			//ft_printf("FREE_FINIIIIIIIIII\n");
			return ;
		}
	}
	//ft_printf("PAS_TROUVE2\n");
}
