/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   calloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/19 11:09:09 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/20 18:35:00 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	void	*ptr;

	if (debug)ft_printf("\tcalloc(%zu, %zu);\n\t\t", count, size);
	ptr = malloc(count * size);
	if (debug)ft_printf("//calloc[%p];\n", ptr);
	if (ptr)
		ft_memset(ptr, 0, /*NEXT_MULTIPLE(count * size, 16)*/count*size);
	return (ptr);
}
