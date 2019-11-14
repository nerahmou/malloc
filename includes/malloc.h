/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/14 10:24:51 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/14 15:38:05 by nerahmou    ###    #+. /#+    ###.fr     */
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

# define FREE_LARGE(head, region, g_op) free_region(head, region, g_op)

# define ALIGNEMENT 16

# define BINS_NUMBER (SMALL_MAX_SIZE / 16) - 1// 255 cases (32->4096)

# define BIN_INDEX(size) (size - CHUNK_HEAD_SIZE) / ALIGNEMENT - 1

# define TINY_BINS_LIMIT (TINY_MAX_SIZE / ALIGNEMENT) - 1

# define SMALL_BINS_LIMIT (SMALL_MAX_SIZE / ALIGNEMENT) - 1

/*
 * Les MAX_SIZE inclues les HEADERS de CHUNK
 * 
 * 32	>=	TINY_MALLOC		<= 512
 * 528	>=	SMALL_MALLOC	<= 4096
 * 4096	>	LARGE_MALLOC
 *
 * exemple un malloc(496) sera dans les TINY car:
 *		- 496 multiple de 16
 *		- 496 + Header = 512
 *
 *	un malloc(497) sera dans les SMALL car:
 *		- 497 est arrondi au multiple de 16 superieur -> 512
 *		- 512 + header = 528
 */
# define TINY_MAX_SIZE 512
# define TINY_REGION_SIZE (TINY_MAX_SIZE * 500)

# define SMALL_MAX_SIZE 4096
# define SMALL_REGION_SIZE (SMALL_MAX_SIZE * 100)

# define LARGE_MAX_SIZE UINT64_MAX - 1
# define LARGE_REGION_SIZE (SEG_HEAD_SIZE)


# define SEG_HEAD_SIZE sizeof(t_region)
# define CHUNK_HEAD_SIZE 16
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

# define CHUNK_SIZE(chunk) (long) (chunk == NULL ? 0 : chunk->size)

# define GET_APPROPRIATE_region_TYPE(offset) (t_region**)(&g_heap.tiny_region + offset)
/*
 * Arrondi au multiple de 16 superieur
 * */
# define GET_NEXT_MULTIPLE(size) ((size - 1) / 16 * 16 + 16)

/*
 * Retourne la taille totale de l'allocation demandée en incluant le header du chunk
 */

# define GET_REQUIRED_SIZE(size) GET_NEXT_MULTIPLE(size) + CHUNK_HEAD_SIZE

# define MOVE_CHUNK_ADDR(chunk, size) chunk + size / ALIGNEMENT

# define CHUNK_DATA_SIZE(chunk) chunk->size + (long)chunk - (long)chunk - CHUNK_HEAD_SIZE


/*
 * 
 */

# define AVAILABLE_SPACE(reg) reg->u_u.size - SEG_HEAD_SIZE

# define IS_FIRST_CHUNK(seg, size) seg->u_u.available_space == size

# define CHUNK_IN_SEG(addr, seg, size) addr > seg && addr <= seg + size

# define GET_FIRST_CHUNK(addr) CH_PTR (addr + (addr != NULL))

# define CHUNK_DATA(addr) &(addr->u_u.data)

# define SET_CHUNK_POS(s, s_size) CH_PTR (s + (SEG_HEAD_SIZE + s_size - s->u_u.available_space))

# define UPDATE_CHUNK_SIZE(new_size) new_size - CHUNK_HEAD_SIZE


# define NEXT_CHUNK(ch) CH_PTR ((long)ch + ch->size)


# define IS_PREV_FREE(ch, max) ch->prev  && !ch->prev->in_use && (ch->size + ch->prev->size) <= max // a verifier

# define IS_NEXT_FREE(ch, max) (ch->next_size && (NEXT_CHUNK(ch))->in_use == false && ch->size + ch->next_size <= max)

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
typedef struct s_region			t_region;
typedef struct s_chunk				t_chunk;
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

struct	s_chunk
{
	t_chunk		*prev;
	size_t		next_size:28; // Meme role qu'un pointeur mais ne prends que 2 octets.
						 // Utile pour eviter d'avoir un header a 48 octets
	size_t		size:28;
	bool		in_use;
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
	union
	{
		size_t		available_space; // For tiny and small
		size_t		size; //For large chunks
	} u_u;
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
	size_t		region_size;
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
t_chunk	*get_chunk_from_bin(size_t, size_t);

void	show_alloc_mem();
void	show_bins();
void ft_putnbr_base(size_t nbr, const char *base, size_t base_len);


#endif
