/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 11:50:21 by nerahmou    ###    #+. /#+    ###.fr     */
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
	new = (t_chunk*)((size_t)region + REG_HEAD_SIZE + (region_size - region->space));
	if ((size_t)new != (size_t)region + REG_HEAD_SIZE)
	{
		prev_chunk = (t_chunk*)((size_t)region + REG_HEAD_SIZE);
		while (prev_chunk->next != NULL)
			prev_chunk = prev_chunk->next;
		prev_chunk->next = new;
		new->prev = prev_chunk;
	}
	else
		new->prev = NULL;
	new->size = size;
	new->next = NULL;
	new->in_use = true;
	region->space -= size;
	//new->next_size = 0;
	return (new);
}

/*
 * LARGE_size = REQUIRED_SIZE + SEG_HEAD_SIZE;
 */
void	*new_region(t_region **head, size_t len)
{
	t_region	*new_region = NULL;

	new_region = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
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
		region_size = required(size, REG_HEAD_SIZE, 4096);
		head = &(g_heap.large_region);
	}
	region = *head;
	while (region)
	{
		if (region->space - REG_HEAD_SIZE >= size)
		{
			if (head == &(g_heap.large_region))
				ft_printf("PROBLEM\n");
			return (region);
		}
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

size_t round_up(size_t size, size_t to_round)
{
	return (((size + to_round - 1) & (0xFFFFFFFFFFFFFFFF - (to_round - 1))));
}

size_t required(size_t size, size_t head, size_t mult)
{
	return (round_up(size + head, mult));
}

void	*malloc(size_t size)
{

	//ft_printf("malloc(%zu)\n", size);
	void *addr;

	addr = NULL;
	if (size == 0)
		size = 16;
	if (size != 0)
	{
		size = required(size, CHUNK_HEAD_SIZE, 16);
		addr = place_chunk(size);
	}
	return (addr);
}

