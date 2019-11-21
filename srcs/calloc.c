/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   calloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/19 11:09:09 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/21 17:22:28 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	void	*ptr;

	ft_printf("calloc(%zu, %zu);\n\t", count, size);
	ptr = malloc(count * size);
	if (ptr)
		ft_memset(ptr, 0, DATA_SIZE((CHUNK_HEADER(ptr))));
	ft_printf("//calloc[%p] 0_len{%zu};\n", ptr, DATA_SIZE((CHUNK_HEADER(ptr))));
	return (ptr);
}
