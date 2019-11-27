/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   realloc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/14 16:19:14 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/27 18:11:27 by nerahmou    ###    #+. /#+    ###.fr     */
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
	void	*data = NULL;

	data = malloc(size);
	if (data)
	{
		ft_memmove(data, ptr, size - needed_size);
		free(ptr);
	}
	return (data);
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
	region = get_the_region(*head, ptr, region_size);
	needed_size = size - DATA_SIZE(chunk);
	if (region_size == LARGE_REGION_SIZE && AVAILABLE_SPACE(region, needed_size))
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
	size = NEXT_MULTIPLE(size, 16);
	region = get_the_region(*head, ptr, region_size);
	needed_size = size - DATA_SIZE(chunk);
	next_chunk = chunk->next;
	if (next_chunk == NULL && AVAILABLE_SPACE(region, needed_size))
	{
		chunk->size += needed_size;
		region->space -= needed_size;
	}
	else if (next_chunk && next_chunk->in_use == false && next_chunk->size == needed_size && chunk->size + needed_size <= max_size)
	{
		next_chunk = pop_specific((t_freed*)next_chunk);
		chunk = merge_chunks(chunk, next_chunk);
	}
	else if (next_chunk && next_chunk->in_use == false &&(long)next_chunk->size - (long)needed_size >= MIN_BIN_SIZE && chunk->size + needed_size <= max_size)
	{
		next_chunk = pop_specific((t_freed*)next_chunk);
		next_chunk = split_bin_elem(next_chunk, next_chunk->size, needed_size);
		chunk = merge_chunks(chunk, next_chunk);
	}
	else
		return (rellocate_chunk(ptr, size, needed_size));
	return (ptr);
}


void	*realloc(void *ptr, size_t size)
{
	t_region		**head;
	t_region		*region;
	t_chunk			*chunk = NULL;
	void			*addr = NULL;
	
	if (ptr == NULL)
		return (malloc(size));
	head = is_valid_ptr(ptr);
	if (head == NULL)
		return (NULL);
	chunk = CHUNK_HEADER(ptr);
	if (size > chunk->size - CHUNK_HEAD_SIZE)
	{
		if (chunk->size <= SMALL_MAX_SIZE)
			return (realloc_small(head, chunk, ptr, size));
		/*addr = malloc(size);
		ft_memmove(addr, ptr, DATA_SIZE(chunk));
		free(ptr);
		return addr;
	*/	return (realloc_large(head, chunk, ptr, size));
	}
	return (ptr);
}
