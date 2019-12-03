/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   calloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/19 11:09:09 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/03 16:18:34 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (ptr)
		ft_memset(ptr, 0, next_multiple(count * size, 16));
	return (ptr);
}
