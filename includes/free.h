/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/05 15:37:35 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/06 17:44:42 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef FREE_H

/*
******************DEFINES****************
*/
# define FREE_H
# define BINS_NUMBER 510
# define MAX_BINS_INDEX 509

/*
** MIN_BIN_SIZE is the minimum acceptable size to store in the first bin
*/
# define MIN_BIN_SIZE 48

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
bool	unused_region(t_region *region);
void	clear_unused_bins(t_region *region);
void	show_bins(size_t nb);

/*
******************GLOBALS****************
*/
extern t_chunk	*g_bins[BINS_NUMBER];

#endif
