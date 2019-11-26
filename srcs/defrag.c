/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   defrag.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/08 16:30:24 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/26 18:51:14 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "free.h"

bool	unused_region(t_region *region)
{
	t_chunk *chunk;
	
	
	//show_alloc_mem();
	//show_bins(10);
	//ft_printf("\tnused_region(1) region=[%p]\n", region);
	chunk = FIRST_CHUNK(region);
	//ft_printf("\tunused_region(2) chunk=[%p]\n", chunk);
	while (chunk && chunk->in_use == false)
	{
	//	ft_printf("\tunused_region(3) chunk=[%p]\n", chunk);
		chunk = chunk->next;
	//	ft_printf("\tunused_region(4) chunk=[%p]\n", chunk);
	}
	//ft_printf("\tunused_region(5) chunk=[%p]\n", chunk);
	return (chunk ==  0);
}

t_chunk	*merge_prev_free(t_chunk *freed_chunk)
{
	t_chunk	*bin_elem;

//	ft_printf("\n\tMERGE_PREVIOUS\n");
	bin_elem = pop_specific(freed_chunk->prev);
//	ft_printf("\tmerge_prev(2) APRES POP %p\n", bin_elem);
	bin_elem->size += freed_chunk->size;
//	ft_printf("\tmerge_prev(3) APRES POP bin_elem->size%zu\n", bin_elem->size);
	bin_elem->next = freed_chunk->next;
//	ft_printf("\tmerge_prev(4) bin_elem->next=[%p]\n", bin_elem->next);
	if (freed_chunk->next)
		freed_chunk->next->prev = bin_elem;
	return (bin_elem);
}

t_chunk	*merge_next_free(t_chunk *freed_chunk)
{
	t_chunk	*bin_elem;

//	ft_printf("\n\tMERGE_NEXT\n");
	bin_elem = pop_specific(freed_chunk->next);
//	ft_printf("\tmerge_next(2) APRES POP bin_elem=[%p]|freed_chunk->next=[%p]\n", bin_elem, freed_chunk->next);
	freed_chunk->size += bin_elem->size;
//	ft_printf("\tmerge_next(3) APRES POP bin_elem->size%zu\n", freed_chunk->size);
	freed_chunk->next = bin_elem->next;
//	ft_printf("\tmerge_next(4) bin_elem->next=[%p]\n", freed_chunk->next);
	if (/*freed_chunk->next*/bin_elem->next)
		bin_elem->next->prev = freed_chunk;//freed_chunk->next->prev = freed_chunk;
	return (freed_chunk);
}

bool	defrag(t_region *region, t_chunk **chunk)
{
	t_chunk			*freed_chunk;
	unsigned short	bin_size_limit;

	/*freed_chunk = *chunk;
	if (freed_chunk->size <= TINY_MAX_SIZE)
		bin_size_limit = TINY_MAX_SIZE;
	else
		bin_size_limit = SMALL_MAX_SIZE;
	while (freed_chunk->prev && freed_chunk->prev->in_use == false
			&& freed_chunk->size + freed_chunk->prev->size <= bin_size_limit)//IS_PREV_FREE(freed_chunk) && INF_MAX_BIN(freed_chunk, PREV_CHUNK(freed_chunk), bin_size_limit))
			{
	//			ft_printf("defrag(1) - freed size=%zu|prev size=%zu|les deux=%zu\n", freed_chunk->size, freed_chunk->prev->size, freed_chunk->size + freed_chunk->prev->size);
	//			ft_printf("defrag(2) - freed_chunk=[%p]|freed_chunk->prev=[%p]\n",freed_chunk,  freed_chunk->prev);
			//	show_bins(70);
				freed_chunk = merge_prev_free(freed_chunk);
	//			ft_printf("defrag(3) - freed_chunk(AFTER MERGE_PREV)=[%p]\n", freed_chunk);
			}
	//ft_printf("defrag(4) FIN freed_chunk=[%p]\n", freed_chunk);
	while (freed_chunk->next && freed_chunk->next->in_use == false && freed_chunk->size + freed_chunk->next->size <= bin_size_limit)//IS_NEXT_FREE(freed_chunk) && INF_MAX_BIN(freed_chunk, (NEXT_CHUNK(freed_chunk)), bin_size_limit))
	{
		//ft_printf("defrag(4) - freed size=%zu|next size=%zu|les deux=%zu\n", freed_chunk->size, freed_chunk->next->size, freed_chunk->size + freed_chunk->next->size);
		//ft_printf("defrag(5) - freed_chunk=[%p]|freed_chunk->next=[%p]\n",freed_chunk, freed_chunk->next);
		freed_chunk = merge_next_free(freed_chunk);
		//ft_printf("defrag(6) - freed_chunk(AFTER MERGE_PREV)=[%p]\n", freed_chunk);
	}
	*chunk = freed_chunk;
	*/return (unused_region(region));
}
