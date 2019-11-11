/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 15:12:28 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/11 15:41:29 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"

t_heap	g_heap = {NULL, NULL, NULL};

t_chunk	*g_bins[BINS_NUMBER] = {NULL};

t_op g_op[4] = {
	{TINY_MAX_SIZE, TINY_SEGMENT_SIZE, TINY_SEGMENT_OFFSET, false},
	{SMALL_MAX_SIZE, SMALL_SEGMENT_SIZE, SMALL_SEGMENT_OFFSET, false},
	{LARGE_MAX_SIZE, LARGE_SEG_SIZE, LARGE_SEGMENT_OFFSET, true},
	{0, 0, 0, NULL}
};

