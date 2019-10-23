/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/23 15:54:45 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

t_heap	g_heap;



void	*new_segment(size_t len)
{
	return (MMAP(NULL, len));
}




/*void	*large_chunk(t_segment *segment, int segment_size, size_t size)
{
	t_segment	*tmp;
	t_segment	*new;

	tmp = segment;
	while (tmp->next)
		tmp = tmp->next;
	new = MMAP(NULL, sizeof(t_segment));
	new->next = NULL;
	new->first_chunk = MMAP(NULL, sizeof(t_chunk));
	new->last_chunk = MMAP(NULL, sizeof(t_chunk));

		return NULL;
}
*/
void	*small_chunk(t_segment *segment, int segment_size, size_t size)
{
		return NULL;
}

void	*new_chunk(size_t size)
{
	void		*addr;
	short		i;
	int			max_chunk_size;
	int			segment_size;
	t_segment	*segment;

	addr = NULL;
	i = 0;
	while (g_op[i].max_chunk_size)
	{
		max_chunk_size = g_op[i].max_chunk_size;
		if ((size / max_chunk_size) == 0)
		{
			segment_size = g_op[i].segment_size;
			segment = g_op[i].segment;
			addr = g_op[i].ptr_func(segment, size, segment_size);
		}
		i++;
	}
	return (addr);
}

void	*malloc(size_t size)
{
	void	*addr;
	
	addr = NULL;
	addr = new_chunk(size);
	return (addr);
}

