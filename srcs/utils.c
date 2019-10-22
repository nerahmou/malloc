/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/22 15:35:36 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

static long max_virtual_memory()
{
	struct rlimit rlim;

	getrlimit(RLIMIT_AS, &rlim);
	return (rlim.rlim_max);
}

t_op g_op[3] = {
	{TINY_MAX_SIZE, TINY_SEGMENT_SIZE, g_heap.tiny_segment, &small_chunk},
	{SMALL_MAX_SIZE, SMALL_SEGMENT_SIZE,  g_heap.small_segment, &small_chunk},
	{max_virtual_memory(), 0, g_heap.large_segment, &large_chunk},
	{0, 0, NULL, NULL}
};
