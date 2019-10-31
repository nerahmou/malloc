/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/31 16:11:21 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/31 16:45:58 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"


t_segment	*valid_addr(void *addr)
{
	t_segment	**head;
	t_segment	*segment;
	size_t		segment_end;
	short		i;

	i = 3;
	while (--i)
	{
		head = GET_APPROPRIATE_SEGMENT_TYPE(g_op[i].offset);
		segment = *head;
		while(segment)
		{
			GET_SEGMENT_LIMIT(segment, g_op[i].is_large, g_op[i].segment_size);
			if ((t_segment *)addr > segment && (t_segment *) addr <= segment + g_op[i].segment_size)
				;
		}
	}
	return NULL;
}

void	free(void *addr)
{
	if (addr != NULL)
	{
		valid_addr(addr);
	}

}
