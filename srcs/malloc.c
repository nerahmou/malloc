/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 14:15:38 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>
bool debug=0;

t_chunk	*place_in_region(t_region *region, size_t size)
{
	t_chunk		*prev_chunk = NULL;
	t_chunk		*new = NULL;
	size_t		region_size;

	if (size <= TINY_MAX_SIZE)
		region_size = TINY_REGION_SIZE;
	else if (size <= SMALL_MAX_SIZE)
		region_size = SMALL_REGION_SIZE;
	else
		region_size = region->space;
	new = SET_CHUNK_POS(region, region_size);
	if (!IS_FIRST_CHUNK(region, new))
	{
		prev_chunk = FIRST_CHUNK(region);
		while (prev_chunk->next_size != 0)
		{
			if (prev_chunk->next_size != (NEXT_CHUNK(prev_chunk))->size)
				ft_printf("KO");
			prev_chunk = NEXT_CHUNK(prev_chunk);
		}
		prev_chunk->next_size = new->size;
		new->prev = prev_chunk;
	}
	else
		new->prev = NULL;
	new->size = size;
	new->next_size = 0;
	new->in_use = true;
	region->space -= size;
	return (new);
}

/*
 * LARGE_size = REQUIRED_SIZE + SEG_HEAD_SIZE;
 */
void	*new_region(t_region **head, size_t len)
{
	t_region	*new_region = NULL;

	new_region = MMAP(len);
	if (new_region == MAP_FAILED)
		return (NULL);
	if (*head == NULL)
		*head = new_region;
	new_region->space = len; // Gere les deux cas vu que c'est uen union
	new_region->next = NULL;
	return (new_region);
}


/*
 * get_region():	Find and return a region with enough space for the 
 *					size. If not found, call new_region()
 */
t_region	*get_region(size_t size)
{
	t_region	**head = NULL;
	t_region	*region = NULL;
	t_region	*previous_region = NULL;
	size_t		region_size;

	previous_region = NULL;
	if (size <= TINY_MAX_SIZE)
	{
		region_size = TINY_REGION_SIZE;
		head = &(g_heap.tiny_region);
	}
	else if (size <= SMALL_MAX_SIZE)
	{
		region_size = SMALL_REGION_SIZE;
		head = &(g_heap.small_region);
	}
	else
	{
		region_size = REQUIRED_SIZE(size, REG_HEAD_SIZE, 4096);
		head = &(g_heap.large_region);
	}
	region = *head;
	while (region)
	{
		if (AVAILABLE_SPACE(region, size))
			return (region);
		previous_region = region;
		region = region->next;
	}
	if (region == NULL)
	{	
		region = new_region(head, region_size);
		if (previous_region != NULL)
			previous_region->next = region;
	}
	return (region);
}


void	*place_chunk(size_t size)
{
	t_chunk		*new_chunk = NULL;
	t_region	*region = NULL;

	region = get_region(size);
	if (region == NULL)
		return (NULL);
	new_chunk = place_in_region(region, size);
	return (&(new_chunk->data));
}

void	*malloc(size_t size)
{
	void *addr;

	addr = NULL;
	if (size == 0)
		++size;
	size = REQUIRED_SIZE(size, CHUNK_HEAD_SIZE, 16);
	addr = place_chunk(size);
	return (addr);
}

