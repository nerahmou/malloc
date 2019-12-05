/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   region.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/03 16:01:20 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/05 13:10:46 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void		*new_region(t_region **head, size_t len)
{
	t_region	*new_region;

	new_region = mmap(NULL, len, PROT_OPTS, MAP_FLAGS, -1, 0);
	if (new_region == MAP_FAILED)
		return (NULL);
	if (*head == NULL)
		*head = new_region;
	new_region->head = head;
	new_region->space = len;
	new_region->size = len;
	new_region->next = NULL;
	if (len == TINY_REGION_SIZE)
		new_region->max_chunk_size = TINY_MAX_SIZE;
	else if (len == SMALL_REGION_SIZE)
		new_region->max_chunk_size = SMALL_MAX_SIZE;
	return (new_region);
}

t_region	**get_appropriate_region_type(size_t size)
{
	if (size <= TINY_MAX_SIZE)
		return (&(g_heap.tiny_region));
	if (size <= SMALL_MAX_SIZE)
		return (&(g_heap.small_region));
	return (&(g_heap.large_region));
}

t_region	*find_space_in_region(t_region *reg, t_region **prev, size_t size)
{
	*prev = NULL;
	while (reg)
	{
		if (reg->space - REG_HEAD_SIZE >= size)
			return (reg);
		*prev = reg;
		reg = reg->next;
	}
	return (reg);
}

t_region	*get_region_to_place_chunk(size_t size)
{
	t_region	**head;
	t_region	*region;
	t_region	*previous_region;
	size_t		region_size;

	head = get_appropriate_region_type(size);
	region = find_space_in_region(*head, &previous_region, size);
	if (region == NULL)
	{
		if (size <= TINY_MAX_SIZE)
			region_size = TINY_REGION_SIZE;
		else if (size <= SMALL_MAX_SIZE)
			region_size = SMALL_REGION_SIZE;
		else
			region_size = required_size(size, REG_HEAD_SIZE, PAGE_SIZE);
		region = new_region(head, region_size);
		if (previous_region != NULL)
			previous_region->next = region;
	}
	return (region);
}
