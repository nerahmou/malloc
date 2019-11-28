/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/05 15:37:35 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/28 17:49:44 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef FREE_H
# define FREE_H

# include "malloc.h"

# define BINS_NUMBER (unsigned short)((SMALL_MAX_SIZE / 16) - 2)
# define MIN_BIN_SIZE 48

/*
******************FREE****************
*/
void	free(void *ptr);
void	free_region(t_region **head, t_region *region);
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
t_chunk	*g_bins[BINS_NUMBER];


#endif
