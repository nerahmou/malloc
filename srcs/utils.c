/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/23 16:07:02 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_heap g_heap;

static long max_virtual_memory()
{
	struct rlimit rlim;

	getrlimit(RLIMIT_AS, &rlim);
	return (rlim.rlim_max);
}

t_op g_op[4] = {
	{5, TINY_SEGMENT_SIZE, NULL, &small_chunk},
	{SMALL_MAX_SIZE, SMALL_SEGMENT_SIZE,  NULL, &small_chunk},
	{10, 0, NULL, &large_chunk},
	{0, 0, NULL, NULL}
};
