/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/17 16:24:17 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/23 15:36:57 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>
bool debug=0;

t_chunk	*place_in_region(t_region *region, size_t region_size, size_t size)
{
	t_chunk		*prev_chunk;
	t_chunk		*new;

	//new = SET_CHUNK_POS(region, region_size);
	new = FIRST_CHUNK(region);
	while (new->header.size)
		new = CH_PTR((size_t)new + (size_t)new->header.size);
	if (IS_FIRST_CHUNK(region, new) == false)
	{
		//////////ft_printf("\nPAS PREMIER\n");
		prev_chunk = FIRST_CHUNK(region);
		//ft_printf("region=[%p]\nfirst_chunk=[%p]\n\n", region, prev_chunk);
		/*while (prev_chunk->header.next_size != 0)
			prev_chunk = NEXT_CHUNK(prev_chunk);	
		prev_chunk->header.next_size = size;
		new->header.prev = prev_chunk;
		*/while (prev_chunk->header.next)
			prev_chunk = prev_chunk->header.next;
		prev_chunk->header.next = new;
	}
	else
	{
		new->header.prev = NULL;
	}
	new->header.size = size;
	new->header.next_size = 0;
	new->header.in_use = true;
	new->header.next = NULL;
	region->space -= size;
	return (new);
}

/*		if (NEXT_SIZE(prev_chunk) != CHUNK_SIZE(NEXT_CHUNK(prev_chunk)))
				{		
					//show_alloc_mem();
						ft_printf("\nchunk1=[%p]\nchunk2=[%p]\nsize1=[%zu]\nnext_size1=[%zu]\nnext_size2[%zu]\nin_use[%i]",
						prev_chunk,
						NEXT_CHUNK(prev_chunk),
						CHUNK_SIZE(prev_chunk),
						NEXT_SIZE(prev_chunk),
						CHUNK_SIZE(NEXT_CHUNK(prev_chunk)),
						IN_USE(NEXT_CHUNK(prev_chunk)));
				exit(1);}

*/
/*
 * LARGE_size = REQUIRED_SIZE + SEG_HEAD_SIZE;
 */
void	*new_region(t_region **head, size_t len)
{
	t_region	*new_seg;

	new_seg = MMAP(len);
	if (new_seg == MAP_FAILED)
		return (NULL);
	if (*head == NULL)
		*head = new_seg;
	new_seg->next = NULL;
	new_seg->space = len; // Gere les deux cas vu que c'est uen union
	return (new_seg);
}


/*
 * get_region():	Find and return a region with enough space for the 
 *					size. If not found, call new_region()
 */
t_region	*get_region(t_op g_op, size_t size)
{
	t_region	**head;
	t_region	*region;
	t_region	*previous_region;
	size_t		region_size;

	previous_region = NULL;
	head = APPROPRIATE_REGION_TYPE(g_op.offset);
	region = *head;
	region_size = g_op.is_large ? REQUIRED_SIZE(size, 4096) : g_op.reg_size;
	while (region)
	{
		if (AVAILABLE_SPACE(region, size))
			return (region);
		previous_region = region;
		region = region->next;
	}
	if (region == NULL)
	{	
		region = new_region(head, region_size);
		if (previous_region != NULL)
			previous_region->next = region;
	}
	return (region);
}


void	*place_chunk(t_op g_op, size_t size)
{
	t_chunk		*new_chunk;
	t_region	*region;
	size_t		region_size;


	/*new_chunk = get_chunk_from_bin(NULL, size, g_op);
	if (new_chunk)
	{
		//ft_printf("FROM BIN\n");
		return (CHUNK_DATA(new_chunk));
	}*/
	region = get_region(g_op, size);
	if (region == NULL)
		return (NULL);
	region_size = g_op.is_large ? REQUIRED_SIZE(size, 4096) : g_op.reg_size;
	new_chunk = place_in_region(region, region_size, size);
	return (CHUNK_DATA(new_chunk));
}

void	*malloc(size_t size)
{
	unsigned char	i;
	void *addr;

	i = -1;

	//ft_printf("malloc(%zu);\n", size);
	addr = NULL;
	if (size != 0)
	{
		size = REQUIRED_SIZE(size, 16);
		while (g_op[++i].max_chunk_size)
		{
			if (GOOD_REGION_TYPE(size, g_op[i]))
			{
				//ft_printf("//avant palce\n");
				addr = place_chunk(g_op[i], size);
			//	ft_printf("\t//malloc[%p] size:(%zu);\n", addr, size);
				return (addr);
			}
}
	}
	return (addr);
}

