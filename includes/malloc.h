/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/14 10:24:51 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/19 11:49:02 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef MALLOC_H

/*
** DEFINES
*/
# define MALLOC_H


# define DECI_BASE 10
# define DECI_BASE_STR HEXA_BASE_STR

# define HEXA_BASE 16
# define HEXA_BASE_STR "0123456789ABCDEF"
# define HEXA_PREFIX "0x"

# define PROT_OPTS (PROT_READ | PROT_WRITE)
# define MAP_FLAGS (MAP_ANON | MAP_PRIVATE)
# define MMAP(len) mmap(NULL, len, PROT_OPTS, MAP_FLAGS, -1, 0)

# define MUNMAP(addr, len) munmap(addr, len);

# define PAGE_SIZE getpagesize()


# define ALIGNEMENT 16

# define BINS_NUMBER (unsigned char)((SMALL_MAX_SIZE / 16) - 1)// 255 cases (32->4096)
# define BIN_INDEX(size) (size - CHUNK_HEAD_SIZE) / ALIGNEMENT - 1
# define TINY_BINS_LIMIT (TINY_MAX_SIZE / ALIGNEMENT) - 1
# define SMALL_BINS_LIMIT (SMALL_MAX_SIZE / ALIGNEMENT) - 1

# define FREE_LARGE(head, region, g_op) free_region(head, region, g_op)
/*
 *
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
 *		- 497 est arrondi au multiple de 16 superieur -> 512
 *		- 512 + header = 528
 */

# define TINY_NEXT_MULT (CHUNK_HEAD_SIZE)
# define TINY_MAX_SIZE (CHUNK_HEAD_SIZE + 496)
# define TINY_REGION_SIZE ((TINY_MAX_SIZE << 3) * 300)

# define SMALL_NEXT_MULT (CHUNK_HEAD_SIZE)
# define SMALL_MAX_SIZE (CHUNK_HEAD_SIZE + 4080)
# define SMALL_REGION_SIZE (SMALL_MAX_SIZE * 200)

/*
 * LARGE_NEXT_MULT = 4088; Etant donné qu'il est geré différement:
 *	(La taille de region est égale au next_multiple), donc on deduit la taille d'un header
 *
 * LARGE_REGION_SIZE = 32; 
 *	Necessaire pour CHUNK_IN_SEG(reg, ptr, LARGE_REGION_SIZE) etant donné qu'il n'y a
 *	qu'un seul element dans un LARGE_MALLOC, on verifie si ptr est a l'adresse
 *	du premier chunk.
 */
# define LARGE_NEXT_MULT (SMALL_MAX_SIZE)
# define LARGE_MAX_SIZE UINT64_MAX - 1
# define LARGE_REGION_SIZE 32

# define SEG_HEAD_SIZE sizeof(t_region)
# define CHUNK_HEAD_SIZE sizeof(t_header)

# define CH_PTR (t_chunk*)
/*
 * ENUMS
 *
 *	e_region_OFFSET_TYPE: Utilisé pour acceder au bon type de region dans la
 *	g_heap
 * */
enum e_region_OFFSET_TYPE{
	TINY_region_OFFSET,
	SMALL_region_OFFSET,
	LARGE_region_OFFSET
};

/*
 * Recupere le bon region a l'interieur de la heap en fonction en se servant d'un offset definit
 * dans le tableau global g_op*/
# define GOOD_region_TYPE(size, g_op) size / (g_op.max_chunk_size + 1) == 0

# define GET_CHUNK_HEADER(addr) CH_PTR (addr - CHUNK_HEAD_SIZE)

# define GET_NEXT_FREE(chunk) (CH_PTR (chunk->next_free))

# define GET_PREV_FREE(chunk) (CH_PTR (chunk->u_u.prev_free))

# define GET_NEXT_CHUNK(chunk) CH_PTR ((long)chunk + CHUNK_SIZE(chunk))

# define CHUNK_SIZE(chunk) (size_t) (chunk == NULL ? 0 : chunk->header.size)

# define GET_APPROPRIATE_REGION_TYPE(offset) (t_region**)(&g_heap.tiny_region + offset)
/*
 * Arrondi au multiple de 16 superieur
 * */
# define GET_NEXT_MULTIPLE(size, mult) ((size - 1) / mult * mult + mult)

/*
 * Retourne la taille totale de l'allocation demandée en incluant le header du chunk
 */

# define GET_REQUIRED_SIZE(size, mult) GET_NEXT_MULTIPLE(size + SEG_HEAD_SIZE, mult)

# define MOVE_CHUNK_ADDR(chunk, size) chunk + size / ALIGNEMENT

# define CHUNK_DATA_SIZE(chunk) (chunk->header.size - CHUNK_HEAD_SIZE)


/*
 * 
 */

# define AVAILABLE_SPACE(reg, size) (reg->space - SEG_HEAD_SIZE) >= size

# define IS_FIRST_CHUNK(reg, chunk) ((size_t)reg + SEG_HEAD_SIZE) == (size_t)chunk

# define CHUNK_IN_SEG(addr, seg, size) addr > seg && addr <= seg + size

# define GET_FIRST_CHUNK(addr) CH_PTR (addr + 1)

# define CHUNK_DATA(addr) &(addr->u_u.data)

# define SET_CHUNK_POS(r, r_size) CH_PTR ((long)r + (r_size - r->space) + CHUNK_HEAD_SIZE)

# define UPDATE_CHUNK_SIZE(new_size) new_size - CHUNK_HEAD_SIZE


# define NEXT_CHUNK(ch) CH_PTR ((long)ch + ch->header.size)


# define IS_PREV_FREE(ch, max) ch->header.prev  && !ch->header.prev->header.in_use && (ch->header.size + ch->header.prev->header.size) <= max // a verifier

# define IS_NEXT_FREE(ch, max) (ch->header.next_size && (NEXT_CHUNK(ch))->header.in_use == false && ch->header.size + ch->header.next_size <= max)

# define IS_SPLITTABLE(chunk, size) (long)CHUNK_SIZE(chunk) - (long)size >= 32

/*
** INCLUDES
*/

# include <stdlib.h>
# include <stdbool.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <unistd.h>

/*
 * PROTOTYPES
 */

/*
 *void	*small_chunk(): Handle the tinies and smalls mallo
 */
void	*small_chunk();
void	*large_chunk();


/*
** TYPE_DEFS
** - t_heap: Structure global gerant la memoire
** - t_region:	Represente un region de memoire realiser a l'aide de mmap()
**				comprenant: les tinies, les smalls et les larges
** - t_chunk: Un troncon de memoire comprenant: Metadata + data
*/
typedef struct s_private_memory		t_private_memory;
typedef struct s_heap				t_heap;
typedef struct s_region				t_region;
typedef struct s_chunk				t_chunk;
typedef struct s_header				t_header;
typedef struct s_op					t_op;


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

struct	s_header
{
	t_chunk		*prev;
	size_t		size:48;
	size_t		next_size:15; // Meme role qu'un pointeur mais ne prends que 2 octets.
	bool		in_use:1;
};

struct	s_chunk
{
	t_header header;
	union
	{
		void *data;
		void *prev_free;
	} u_u;
	void *next_free;
};

struct	s_region
{
	t_region	*next;
	size_t		space;
};

struct	s_heap
{
	t_region			*tiny_region;
	t_region			*small_region;
	t_region			*large_region;
};

struct s_op
{
	size_t		max_chunk_size;
	size_t		reg_size;
	size_t		offset:56;
	bool		is_large;
	char		*region_name;
};

/*
*****************GLOBAL_VARS****************
*/

extern t_heap	g_heap;
extern t_op		g_op[4];
/*
 * Corbeilles utilisé pour stocker l'addresse des malloc free pour les
 * reutiliser sans parcourir l'ensemble d'un region
 */
extern t_chunk	*g_bins[BINS_NUMBER];


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
void	free_region(t_region **head, t_region *region, t_op g_op);
bool	defrag(t_region *region, t_chunk **chunk, t_op g_op);
void	update_bins(t_region *region);
t_chunk *pop(size_t size, t_chunk *chunk);
void	push(t_chunk *chunk);
t_chunk	*get_chunk_from_bin(t_chunk *, size_t, t_op);

void	show_alloc_mem();
void	show_bins();
void ft_putnbr_base(size_t nbr, const char *base, size_t base_len);

t_chunk	*split_bin_elem(t_chunk *chunk, size_t bin_size, size_t size);
unsigned char	is_valid_ptr(void*);
t_region	*get_the_region(t_region *region, void *ptr, unsigned char i);
void	*ft_memcpy(void*, const void*, size_t);
void	*ft_memset(void*, int, size_t);

#endif
