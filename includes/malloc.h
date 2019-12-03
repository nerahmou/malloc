/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*   Created: 2019/10/14 10:24:51 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 09:57:10 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef MALLOC_H

/*
** DEFINES
*/

/*
 * 32B	>=	TINY_REGION		<= 512B
 * 528B	>=	SMALL_REGION	<= 4096B
 * 5008	>	LARGE_REGION
 */

/* malloc(16)	>=	TINY_MALLOC		<= malloc(496)
 * malloc(512)	>=	SMALL_MALLOC		<= malloc(4080)
 * malloc(4096)	>	LARGE_MALLOC
 *
 * exemple un malloc(496) sera dans les TINY car:
 *		- 496 multiple de 16
 *		- 496 + Header = 512
 *
 *	un malloc(497) sera dans les SMALL car:
 PPPPPP*		- 497 est arrondi au multiple de 16 superieur -> 512
 *		- 512 + header = 528
 */
/*PPPPPP
 * LARGE_NEXT_MULT = 4088; Etant donné qu'il est geré différement:
 *	(La taille de region est égale au next_multiple), donc on deduit la taille d'un header
 *
 * LARGE_REGION_SIZE = 32; 
 *	Necessaire pour CHUNK_IN_REG(reg, ptr, LARGE_REGION_SIZE) etant donné qu'il n'y a
 *	qu'un seul element dans un LARGE_MALLOC, on verifie si ptr est a l'adresse
 *	du premier chunk.
 */
/*
** STRUCTURES
**	
**	s_chunk (16 octets):	Chunk of memory in a region
**	{
**		size:	Taille du chunk actuel (prend en compte *data seulement)
**		in_use:	Si le chunk est utilisé (=non free);
**		*data:	Zone des donnees renvoyée par malloc()
**	}
**	
**	s_region (24 octets):	region of memory in the heap
**	{
**		first_chunk:	Point to the first chunk in the region
**		last_chunk:		Point to the "top chunk" which represent the available space at the end of the region
**		next:			Point to the next region
**	}
**
**	s_heap (24 octets):	Handle the heap memory of the processus
**	{
**		tiny:	Point to the list of TINY regions
**		small:	Point to the list of SMALL regions
**		large:	Point to the list of LARGE regions
**	}
**
**	s_op (24 octets):	Fait correspondre la fonction a appeler en fonction de la taille du malloc demandée
**	{
**	}
*/

# define MALLOC_H

# define DECI_BASE 10
# define HEXA_BASE 16

# define HEXA_BASE_STR "0123456789ABCDEF"
# define DECI_BASE_STR HEXA_BASE_STR

# define HEXA_PREFIX "0x"

# define PROT_OPTS (PROT_READ | PROT_WRITE)
# define MAP_FLAGS (MAP_ANON | MAP_PRIVATE)
# define MUNMAP_FAILED -1

# define PAGE_SIZE getpagesize()

# define REG_HEAD_SIZE sizeof(t_region)
# define CHUNK_HEAD_SIZE (sizeof(t_chunk) - sizeof(void*))

# define TINY_MAX_SIZE 1024
# define SMALL_MAX_SIZE 8192

# define TINY_REGION_SIZE 4096 * 512
# define SMALL_REGION_SIZE 4096 * 4096
# define LARGE_REGION_SIZE (REG_HEAD_SIZE + CHUNK_HEAD_SIZE)


# define TINY_REGION_HEAD &(g_heap.tiny_region)
# define SMALL_REGION_HEAD &(g_heap.small_region)
# define LARGE_REGION_HEAD &(g_heap.large_region)

# define ALIGNMENT 16
/*
** INCLUDES
*/
# include <stdbool.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <unistd.h>
# include "libft.h"


/*
** TYPE_DEFS
** - t_heap: Structure global gerant la memoire
** - t_region:	Represente un region de memoire realiser a l'aide de mmap()
**				comprenant: les tinies, les smalls et les larges
** - t_chunk: Un troncon de memoire comprenant: Metadata + data
*/
/*
 * ENUMS
 *
 *	e_region_OFFSET_TYPE: Utilisé pour acceder au bon type de region dans la
 *	g_heap
 * */
typedef struct s_heap				t_heap;
typedef struct s_region				t_region;
typedef struct s_chunk				t_chunk;


struct	s_chunk
{
	t_chunk		*prev;
	t_chunk		*next;
	size_t		size;
	size_t		in_use;
	void		*data;
};

struct	s_region
{
	t_region	**head;
	t_region	*next;
	size_t		size;
	unsigned	space;
	unsigned	max_chunk_size;
};

struct	s_heap
{
	t_region			*tiny_region;
	t_region			*small_region;
	t_region			*large_region;
};

extern t_heap	g_heap;


/*
*****************GLOBAL_VARS****************
*/


void	show_alloc_mem(void);
void	*malloc(size_t size);
void	*small_chunk();
void	*large_chunk();
void	*realloc(void *ptr, size_t size);
void	*get_chunk_from_bin(size_t);
void	show_alloc_mem();
t_region	*is_valid_ptr(void*);
bool	ptr_in_region(void*, t_region*, size_t);
t_region	*get_region(t_region *region, void *ptr, size_t size);
size_t	required_size(size_t size, size_t header_size, size_t mult);
t_region	*get_region_to_place_chunk(size_t size);

#endif
