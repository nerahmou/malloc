/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   realloc.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/14 16:19:14 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/14 16:34:21 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	*realloc(void *ptr, size_t size)
{
	t_chunk *chunk;

	if (ptr == NULL)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (malloc(size));
	}
	chunk = GET_CHUNK_HEADER(ptr);
	size = GET_REQUIRED_SIZE(size);
	if (size == chunk->size)
		return (ptr);
	if (chunk->size - size <= sizeof(t_chunk))
	{
		chunk = split_bin_elem(chunk, chunk->size, size);
		return (ptr);
	}
	return (NULL);
}
