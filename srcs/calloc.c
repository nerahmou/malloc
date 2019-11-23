/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   calloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/19 11:09:09 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/23 19:58:57 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	void	*ptr = NULL;
	//ft_printf("calloc(%zu, %zu);\n\t", count, size);
	ptr = malloc(count * size);
	if (ptr)
	{	//memset(ptr, 0, NEXT_MULTIPLE(count*size, 16));
		ft_printf("\n\tcount*size=[%zu] chunk_size=[%zu] data_size=[%zu]\n\n",
				count*size, (CHUNK_HEADER(ptr))->header.size, DATA_SIZE((CHUNK_HEADER(ptr))));
		memset(ptr, 0, DATA_SIZE((CHUNK_HEADER(ptr))));
	}//ft_printf("//calloc[%p] 0_len{%zu};\n", ptr, DATA_SIZE((CHUNK_HEADER(ptr))));
	return (ptr);
}
