/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   realloc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/14 16:19:14 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/19 19:29:32 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_chunk	*merge_chunks(t_chunk *first_chunk, t_chunk *second_chunk)
{
	if (first_chunk == NULL || second_chunk == NULL)
		return (NULL);
	first_chunk->header.size += second_chunk->header.size;
	first_chunk->header.next_size = second_chunk->header.next_size;
	if (second_chunk->header.next_size)
		(GET_NEXT_CHUNK(second_chunk))->header.prev = first_chunk;
	return (first_chunk);
}

void	*rellocate_chunk(void *ptr, size_t size, size_t needed_size)
{
	void	*data;

	data = malloc(size);
	if (data)
	{
		ft_memcpy(data, ptr, size - needed_size);
		free(ptr);
	}
	return (data);
}

void	*realloc_large(unsigned char i, t_chunk *chunk, void *ptr, size_t size)
{
	size_t		needed_size;
	t_region	**head;
	t_region	*region;
	
	head = GET_APPROPRIATE_REGION_TYPE(g_op[i].offset);
	region = get_the_region(*head, ptr, i);
	needed_size = size - CHUNK_DATA_SIZE(chunk);
	if (AVAILABLE_SPACE(region, size))
		return (ptr);
	return (rellocate_chunk(ptr, size, needed_size));
}

void	*realloc_small(t_chunk *chunk, void *ptr, t_op g_op, size_t size)
{
	t_chunk *next_chunk;
	size_t	needed_size;

	needed_size = GET_NEXT_MULTIPLE(size - CHUNK_DATA_SIZE(chunk), 16);
	next_chunk = GET_NEXT_CHUNK(chunk);
	if ((CHUNK_SIZE(next_chunk) == needed_size
			|| IS_SPLITTABLE(next_chunk, needed_size))
				&& CHUNK_SIZE(chunk) + needed_size <= g_op.max_chunk_size)
	{
		next_chunk = get_chunk_from_bin(next_chunk, needed_size, g_op);
		chunk = merge_chunks(chunk, next_chunk);
	}
	else
		return (rellocate_chunk(ptr, size, needed_size));
	if (chunk == NULL)
		return (rellocate_chunk(ptr, size, needed_size));
	return (ptr);
}

void	*realloc(void *ptr, size_t size)
{
	unsigned char	i;
	t_chunk			*chunk;

	ft_printf("\t\trealloc(%p, %zu);\n", ptr, size);
	if (ptr == NULL)
	{
		ft_printf("//PTR NULL			: %p\n\n", ptr);
		show_alloc_mem();
		void *addr = malloc(size);
		show_alloc_mem();
		return (addr);
	}
	i = is_valid_ptr(ptr);
	if (g_op[i].region_name == NULL)
	{
		ft_printf("//MAUVAISE ADDRESS	: %p\n\n", ptr);
		show_alloc_mem();
		void *addr = malloc(size);
		show_alloc_mem();
		return (addr);
		//return (malloc(size));
	}
	if (size == 0)
	{
		show_bins(70);
		free(ptr);
		show_bins(70);
	}
	else
	{
		chunk = GET_CHUNK_HEADER(ptr);
		if (size > CHUNK_DATA_SIZE(chunk))
		{
			if (g_op[i].is_large)
			{
				//ft_printf("realloc(%p, %zu);\n", ptr, size);
				return (realloc_large(i, chunk, ptr, size));
			}
		//	ft_printf("realloc(%p, %zu);\n", ptr, size);
			return (realloc_small(chunk, ptr, g_op[i], size));
		}
	}
	return (ptr);
}
