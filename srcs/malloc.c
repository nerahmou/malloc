/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/23 19:56:39 by nerahmou    ###    #+. /#+    ###.fr     */
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
	//ft_printf("place_in_region(region_size - available_space)=[%zu]\n", region_size - region->space);
	new = (t_chunk*)((size_t)region + REG_HEAD_SIZE + (region_size - region->space));
	if ((size_t)new != (size_t)region + REG_HEAD_SIZE)
	{
		prev_chunk = (t_chunk*)((size_t)region + REG_HEAD_SIZE);
		while (prev_chunk->header.next_size != 0)
		{
			if (prev_chunk->header.next_size != ((t_chunk*)((size_t)prev_chunk + prev_chunk->header.size))->header.size)
				ft_printf("[%p]->[%zu]\n[%p]->[%zu]\n",
						prev_chunk,
						prev_chunk->header.next_size,
						CH_PTR((size_t)prev_chunk + prev_chunk->header.size),
						(CH_PTR((size_t)prev_chunk + prev_chunk->header.size))->header.size);
			prev_chunk = CH_PTR((size_t)prev_chunk + prev_chunk->header.size);//NEXT_CHUNK(prev_chunk);
		}
		prev_chunk->header.next_size = size;
		new->header.prev = prev_chunk;
	}
	else
	{
		new->header.prev = NULL;
	}
	new->header.size = size;
	new->header.next_size = 0;
	new->header.in_use = true;
	region->space -= size;
	return (new);
}

/*
 * LARGE_size = REQUIRED_SIZE + SEG_HEAD_SIZE;
 */
void	*new_region(t_region **head, size_t len)
{
	t_region	*new_seg = NULL;

	//new_seg = MMAP(len);
	new_seg = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (new_seg == MAP_FAILED)
		return (NULL);
	if (*head == NULL)
		*head = new_seg;
	new_seg->space = len; // Gere les deux cas vu que c'est uen union
	new_seg->next = NULL;
	return (new_seg);
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
		head = (t_region**)&(g_heap.tiny_region);
	}
	else if (size <= SMALL_MAX_SIZE)
	{
		region_size = SMALL_REGION_SIZE;
		head = (t_region**)&(g_heap.small_region);
	}
	else
	{
		region_size = required(size, REG_HEAD_SIZE, 4096);
		//region_size = REQUIRED_SIZE(size, REG_HEAD_SIZE, 4096);
		head = (t_region**)&(g_heap.large_region);
	}
	region = *head;
	while (region)
	{
		if (region->space - REG_HEAD_SIZE >= size)
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
	return ((void*)&new_chunk->data);
}

size_t round_up(size_t size, size_t to_round)
{
		return (		((size + to_round - 1) & (0xFFFFFFFFFFFFFFFF - (to_round - 1))));
}

size_t required(size_t size, size_t head, size_t mult)
{
	return (round_up(size + head, mult));
}
void	*malloc(size_t size)
{
	void *addr;

	addr = NULL;
	if (size != 0)
	{
		size = required(size, CHUNK_HEAD_SIZE, 16);
		addr = place_chunk(size);
	}
	return (addr);
}

