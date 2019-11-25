/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/05 15:37:35 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 17:59:47 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef FREE_H
# define FREE_H

# include "malloc.h"


//# define FREE_LARGE(head, region) free_region(head, region)

# define NEXT_FREED(chunk) ((chunk->next_freed))

# define PREV_FREED(chunk) ((chunk->prev_freed))
# define IS_PREV_FREE(ch) PREV_CHUNK(ch) && IN_USE(PREV_CHUNK(ch)) == false// a verifier


# define IS_NEXT_FREE(ch) (NEXT_CHUNK(ch))->size && IN_USE(NEXT_CHUNK(ch)) == false
/*Inferieur a la taille maximal de la categorie de bins*/
# define INF_MAX_BIN(ch1, ch2, max) CHUNK_SIZE(ch1) + CHUNK_SIZE(ch2) <= max // a verifier

# define BINS_NUMBER (unsigned short)((SMALL_MAX_SIZE / 16) - 2)
# define BIN_INDEX(size) (size - CHUNK_HEAD_SIZE) / 16 - 1

/*
******************FREE****************
*/
void	free(void *ptr);
void	free_region(t_region **head, t_region *region);
bool	defrag(t_region *region, t_chunk **chunk);
void	update_bins(t_region *region);
t_chunk *pop(size_t size, t_chunk *chunk);
void	show_bins();
t_chunk	*split_bin_elem(t_chunk *chunk, size_t bin_size, size_t size);
void	push(t_freed *chunk);

/*
 * Corbeilles utilisé pour stocker l'addresse des malloc free pour les
 * reutiliser sans parcourir l'ensemble d'un region
 */
t_freed	*g_bins[BINS_NUMBER];


#endif
