/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/28 17:20:35 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_heap	g_heap = {NULL, NULL, NULL};

t_op g_op[4] = {
	{TINY_MAX_SIZE, TINY_SEGMENT_SIZE, TINY_SEGMENT_OFFSET, &small_chunk},
	{SMALL_MAX_SIZE, SMALL_SEGMENT_SIZE, SMALL_SEGMENT_OFFSET, &small_chunk},
	{LARGE_MAX_SIZE, LARGE_SEGMENT_SIZE, LARGE_SEGMENT_OFFSET, &large_chunk},
	{0, 0, 0, NULL}
};

void	init_heap()
{
}
