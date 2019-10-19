/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/14 10:24:51 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/19 18:40:40 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef MALLOC_H

/*
** DEFINES
*/
# define MALLOC_H

# define PROT_OPTS (PROT_READ | PROT_WRITE)
# define MAP_FLAGS (MAP_ANON | MAP_PRIVATE)
# define MMAP(addr, len) mmap(addr, len, PROT_OPTS, MAP_FLAGS, -1, 0)

# define PAGE_SIZE getpagesize()

# define TINY_ALIGN 16
# define TINY_MAX_SIZE 512
# define TINY_SEGMENT_SIZE (TINY_MAX_SIZE * 500)

# define SMALL_ALIGN 16
# define SMALL_MAX_SIZE 65024
# define SMALL_SEGMENT_SIZE (SMALL_MAX_SIZE * 100)

/*
** INCLUDES
*/
# include <stdlib.h>
# include <stdbool.h>
# include <sys/mman.h>

/*
** TYPE_DEFS
** - t_heap: Structure global gerant la memoire
** - t_segment:	Represente un segment de memoire realiser a l'aide de mmap()
**				comprenant: les tinies, les smalls et les larges
** - t_chunk: Un troncon de memoire comprenant: Metadata + data
*/
typedef struct s_heap		t_heap;
typedef struct s_segment	t_segment;
typedef struct s_chunk		t_chunk;

/*
** STRUCTURES
**	
**	s_chunk (16 octets):	Chunk of memory in a segment
**	{
**		size:	Taille du chunk actuel (prend en compte *data seulement)
**		in_use:	Si le chunk est utilisé (=non free);
**		*data:	Zone des donnees renvoyée par malloc()
**	}
**	
**	s_segment (24 octets):	Segment of memory in the heap
**	{
**		first_chunk:	Point to the first chunk in the segment
**		last_chunk:		Point to the "top chunk" which represent the available space at the end of the segment
**		next:			Point to the next segment
**	}
**
**	s_heap (24 octets):	Handle the heap memory of the processus
**	{
**		tiny:	Point to the list of TINY segments
**		small:	Point to the list of SMALL segments
**		large:	Point to the list of LARGE segments
**	}
**
**
*/


struct	s_chunk
{
	size_t		size:sizeof(size_t) * 8 - 1;
	size_t		in_use:1;
	void		*data;
};

struct	s_segment
{
	t_chunk		*first_chunk;
	t_chunk		*last_chunk;
	t_segment	*next;
};

struct	s_heap
{
	t_segment	*tiny;
	t_segment	*small;
	t_segment	*large;
};

/*
*****************GLOBAL_VARS****************
*/
t_heap g_heap;


void	show_alloc_mem(void);
/*
*****************MALLOC****************
*/
void	*malloc(size_t size);
/*
******************REALLOC****************
*/
void	*realloc(void *ptr, size_t size);
/*
******************FREE****************
*/
void	free(void *ptr);
#endif
