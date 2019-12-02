/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/05 15:37:35 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/02 19:12:24 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef FREE_H
# define FREE_H

# include "malloc.h"

# define BINS_NUMBER ((SMALL_MAX_SIZE - CHUNK_HEAD_SIZE) / ALIGNMENT)
# define MAX_BINS_INDEX BINS_NUMBER - 1
# define MIN_BIN_SIZE (long)(CHUNK_HEAD_SIZE + ALIGNMENT)

/*
******************FREE****************
*/
void	free(void *ptr);
void	free_region(t_region *region);
bool	defrag(t_region *region, t_chunk **chunk);
void	update_bins(t_region *region);
t_chunk *pop(size_t size);
t_chunk *pop_specific(t_chunk *chunk);
void	show_bins();
t_chunk	*split_bin_elem(t_chunk *chunk, size_t bin_size, size_t size);
void	push(t_chunk *chunk);

/*
 * Corbeilles utilisé pour stocker l'addresse des malloc free pour les
 * reutiliser sans parcourir l'ensemble d'un region
 */
extern t_chunk	*g_bins[BINS_NUMBER];


#endif
