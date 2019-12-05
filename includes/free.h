/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/05 15:37:35 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/04 18:53:36 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef FREE_H

/*
******************DEFINES****************
*/
# define FREE_H
# define BINS_NUMBER ((SMALL_MAX_SIZE - CHUNK_HEAD_SIZE) / ALIGNMENT)
# define MAX_BINS_INDEX BINS_NUMBER - 1

/*
** MIN_BIN_SIZE is the minimum acceptable size to store in the first bin
*/
# define MIN_BIN_SIZE (size_t)(CHUNK_HEAD_SIZE + ALIGNMENT)

/*
******************INCLUDES****************
*/
# include "malloc.h"

/*
******************PROTOTYPES****************
*/

/*
** pop(): Get a chunk from an appropriate bin.
** pop_specific(): Get a specific chunk in the bins.
** clear_unused_bins(): Pop all chunks belonging to a freed region.
*/
t_chunk	*pop(size_t size);
t_chunk	*pop_specific(t_chunk *chunk);
void	push(t_chunk *chunk);
bool	defrag(t_region *region, t_chunk **chunk);
void	clear_unused_bins(t_region *region);
t_chunk	*split_bin_elem(t_chunk *chunk, size_t size);

/*
******************GLOBALS****************
*/
extern t_chunk	*g_bins[BINS_NUMBER];

#endif
