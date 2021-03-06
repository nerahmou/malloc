/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   realloc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/14 16:19:14 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/06 17:14:52 by nerahmou    ###    #+. /#+    ###.fr     */
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
	void	*addr;

	addr = malloc(size);
	if (addr)
	{
		ft_memcpy(addr, ptr, size - needed_size);
		free(ptr);
	}
	return (addr);
}

void	*realloc_large(t_region *region, t_chunk *chunk, void *ptr, size_t size)
{
	size_t	needed_size;

	needed_size = size - (chunk->size - (sizeof(t_chunk) - 8));
	if (region->space - sizeof(t_region) >= needed_size)
	{
		chunk->size += needed_size;
		region->space -= needed_size;
		return (ptr);
	}
	return (rellocate_chunk(ptr, size, needed_size));
}

void	*realloc_small(t_region *reg, t_chunk *chunk, void *ptr, size_t size)
{
	t_chunk	*next_chunk;
	size_t	needed_size;

	size = next_multiple(size, ALIGNMENT);
	needed_size = size - (chunk->size - (sizeof(t_chunk) - 8));
	next_chunk = chunk->next;
	if (chunk->size + needed_size <= reg->max_chunk_size)
	{
		if (next_chunk == NULL && reg->space - sizeof(t_region) >= needed_size)
		{
			chunk->size += needed_size;
			reg->space -= needed_size;
			return (ptr);
		}
		else if (next_chunk && next_chunk->in_use == false &&
			chunk->size + next_chunk->size <= reg->max_chunk_size)
		{
			next_chunk = pop_specific(next_chunk);
			chunk = merge_chunks(chunk, next_chunk);
			return (ptr);
		}
	}
	return (rellocate_chunk(ptr, size, needed_size));
}

void	*realloc(void *ptr, size_t size)
{
	t_region	*region;
	t_chunk		*chunk;

	if (ptr == NULL)
		return (malloc(size));
	region = is_valid_ptr(ptr);
	if (region == NULL)
		return (NULL);
	chunk = (t_chunk*)((size_t)ptr - (sizeof(t_chunk) - 8));
	if (size > chunk->size - (sizeof(t_chunk) - 8))
	{
		if (chunk->size <= SMALL_MAX_SIZE)
			return (realloc_small(region, chunk, ptr, size));
		return (realloc_large(region, chunk, ptr, size));
	}
	return (ptr);
}
