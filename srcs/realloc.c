/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   realloc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/14 16:19:14 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/15 12:09:51 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_chunk	*merge_chunks(t_chunk *first_chunk, t_chunk *second_chunk)
{
	first_chunk->size += second_chunk->size;
	first_chunk->next_size = second_chunk->next_size;
	if (second_chunk->next_size)
		(GET_NEXT_CHUNK(second_chunk))->prev = first_chunk;
	return (first_chunk);
}

t_chunk	*realloc_large(void *ptr, size_t size)
{
	t_chunk *new_chunk;

	new_chunk = malloc(size);
	if (new_chunk)
		;//memmove(CHUNK_DATA(new_chunk), ptr, size);
	free(ptr);
	return (new_chunk);
}

void	*realloc(void *ptr, size_t size)
{
	t_chunk *chunk;
	t_chunk *new_chunk;
	t_chunk *bin_elem;

	// verifier ptr est valide
	if (ptr == NULL)
		return (malloc(size));
	if (g_op[is_valid_ptr(ptr)].region_name == NULL)
		return (ptr);
	if (size == 0)
	{
		free(ptr);
		return (malloc(size));
	}
	chunk = GET_CHUNK_HEADER(ptr);
	size = GET_REQUIRED_SIZE(size);
	if (size == chunk->size)
		return (ptr);
	if (size > SMALL_MAX_SIZE)
		return (realloc_large(chunk, size));
	if (chunk->size - size <= sizeof(t_chunk))
	{
		chunk = split_bin_elem(chunk, chunk->size, size);
		return (ptr);
	}
	if (size <= (chunk->size + chunk->next_size) + sizeof(t_chunk))
	{
		bin_elem = pop(0, GET_NEXT_CHUNK(chunk));
		if (bin_elem == NULL)
		{
			new_chunk = malloc(size);
			//memmove(CHUNK_DATA(new_chunk), ptr, size);
			free(ptr);
		}
		if ((size - chunk->size) - bin_elem->size != 0)
			bin_elem = split_bin_elem(bin_elem, bin_elem->size, size - chunk->size);
		chunk = merge_chunks(chunk, bin_elem);
	}
	return (NULL);
}
