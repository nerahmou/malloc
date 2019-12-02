/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   realloc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/14 16:19:14 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/02 17:46:28 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

t_chunk	*merge_chunks(t_chunk *first_chunk, t_chunk *second_chunk)
{
	first_chunk->size += second_chunk->size;
	first_chunk->next = second_chunk->next;
	if (second_chunk->next)
		second_chunk->next->prev = first_chunk;
	return (first_chunk);
}

void	*rellocate_chunk(void *ptr, size_t size, size_t needed_size)
{
	void	*addr = NULL;

	addr = malloc(size);
	if (addr)
	{
		ft_memcpy(addr, ptr, size - needed_size);
		free(ptr);
	}
	return (addr);
}

void	*realloc_large(t_region **head, t_chunk *chunk, void *ptr, size_t size)
{
	size_t		needed_size;
	t_region	*region = NULL;
	void * addr;
	size_t region_size;

	if (head == &(g_heap.tiny_region))
		region_size = TINY_REGION_SIZE;
	else if (head == &(g_heap.small_region))
		region_size = SMALL_REGION_SIZE;
	else
		region_size = LARGE_REGION_SIZE;
	region = get_region(*head, ptr, region_size);
	needed_size = size - (chunk->size - CHUNK_HEAD_SIZE);
	if (region_size == LARGE_REGION_SIZE && (region->space - REG_HEAD_SIZE) >= needed_size)
	{
		chunk->size += needed_size;
		region->space -= needed_size;
		return (ptr);
	}
	else
	{
		addr = rellocate_chunk(ptr, size, needed_size);
		return addr;
	}
	return ptr;
}

void	*realloc_small(t_region **head, t_chunk *chunk, void *ptr, size_t size)
{
	t_region	*region;
	t_chunk *next_chunk;
	size_t	needed_size;
	size_t	max_size;
	size_t region_size;

	if (chunk->size <= TINY_REGION_SIZE)
	{
		max_size = TINY_MAX_SIZE;
		region_size = TINY_REGION_SIZE;
	}else
	{	
		max_size = SMALL_MAX_SIZE;
		region_size = SMALL_REGION_SIZE;
	}
	size = next_multiple(size, 16);
	region = get_region(*head, ptr, region_size);
	needed_size = size - (chunk->size - CHUNK_HEAD_SIZE);
	next_chunk = chunk->next;
	if (chunk->size + needed_size <= max_size)
	{
		if (next_chunk == NULL && (region->space - REG_HEAD_SIZE) >= needed_size)
		{
			chunk->size += needed_size;
			region->space -= needed_size;
		}
		else if (next_chunk && next_chunk->in_use == false)
		{
			next_chunk = pop_specific((t_chunk*)next_chunk);
			//if ((long)next_chunk->size - (long)needed_size >= MIN_BIN_SIZE)
			//	next_chunk = split_bin_elem(next_chunk, next_chunk->size, needed_size);
			chunk = merge_chunks(chunk, next_chunk);
		}
	}
	return (rellocate_chunk(ptr, size, needed_size));
}


void	*realloc(void *ptr, size_t size)
{
	t_region		**head;
	t_chunk			*chunk;

	if (ptr == NULL)
		return (malloc(size));
	head = is_valid_ptr(ptr);
	if (head == NULL)
		return (NULL);
	chunk = (t_chunk*)((size_t)ptr - CHUNK_HEAD_SIZE);
	if (size > chunk->size - CHUNK_HEAD_SIZE)
	{
		if (chunk->size <= SMALL_MAX_SIZE)
			return (realloc_small(head, chunk, ptr, size));
		return (realloc_large(head, chunk, ptr, size));
	}
	return (ptr);
}
