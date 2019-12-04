/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   next_multiple.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/02 19:02:23 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/02 19:11:06 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

size_t	next_multiple(size_t size, size_t mult)
{
	return (((size - 1) / mult * mult + mult));
}