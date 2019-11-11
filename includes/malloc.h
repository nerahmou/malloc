/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/14 10:24:51 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/11 17:14:31 by nerahmou    ###    #+. /#+    ###.fr     */
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
# define MMAP(len) mmap(NULL, len, PROT_OPTS, MAP_FLAGS, -1, 0)

# define MUNMAP(addr, len) munmap(addr, len + SEG_HEAD_SIZE);

# define PAGE_SIZE getpagesize()

# define FREE_LARGE(head, segment, g_op) free_segment(head, segment, g_op)

# define ALIGNEMENT 16

# define BINS_NUMBER (SMALL_MAX_SIZE / 16)

# define BIN_INDEX(size) (size - CHUNK_HEAD_SIZE) / ALIGNEMENT - 1

/*
 * Les MAX_SIZE inclues les HEADERS de CHUNK
 */
# define TINY_MAX_SIZE 512
# define TINY_SEGMENT_SIZE (TINY_MAX_SIZE * 500)

# define SMALL_MAX_SIZE 4096
# define SMALL_SEGMENT_SIZE (SMALL_MAX_SIZE * 100)

# define LARGE_MAX_SIZE UINT64_MAX - 1
# define LARGE_SEG_SIZE (SEG_HEAD_SIZE)


# define SEG_HEAD_SIZE sizeof(t_segment)
# define CHUNK_HEAD_SIZE 16
# define CH_PTR (t_chunk*)
/*
 * ENUMS
 *
 *	e_SEGMENT_OFFSET_TYPE: Utilisé pour acceder au bon type de segment dans la
 *	g_heap
 * */
enum e_SEGMENT_OFFSET_TYPE{
	TINY_SEGMENT_OFFSET,
	SMALL_SEGMENT_OFFSET,
	LARGE_SEGMENT_OFFSET
};

/*
 * Recupere le bon segment a l'interieur de la heap en fonction en se servant d'un offset definit
 * dans le tableau global g_op*/
# define GOOD_SEGMENT_TYPE(size, g_op) (size / g_op.max_chunk_size) == 0

# define GET_CHUNK_HEADER(addr) CH_PTR (addr - CHUNK_HEAD_SIZE)

# define GET_NEXT_CHUNK(chunk) CH_PTR ((long)chunk + chunk->size)

# define GET_APPROPRIATE_SEGMENT_TYPE(offset) (t_segment**)(&g_heap.tiny_segment + offset)
/*
 * Arrondi au multiple de 16 superieur
 * */
# define GET_NEXT_MULTIPLE(size) ((size - 1) / 16 * 16 + 16)

/*
 * Retourne la taille totale de l'allocation demandée en incluant le header du chunk
 */

# define GET_REQUIRED_SIZE(size) GET_NEXT_MULTIPLE(size) + CHUNK_HEAD_SIZE

# define MOVE_CHUNK_ADDR(chunk, size) chunk + size / ALIGNEMENT


/*
 * 
 */
# define IS_FIRST_CHUNK(seg, seg_size) (seg->u_u.available_space + SEG_HEAD_SIZE) == seg_size

# define CHUNK_IN_SEG(addr, seg, seg_size) addr > seg && addr <= seg + seg_size

# define LARGE_CHUNK_DATA(addr) (addr + (addr != NULL))

# define CHUNK_DATA(addr) &(addr->u_u.data)

# define SET_CHUNK_POS(s, s_size) CH_PTR (s + (s_size - s->u_u.available_space))

# define UPDATE_CHUNK_SIZE(new_size) new_size - CHUNK_HEAD_SIZE


# define NEXT_CHUNK(ch) CH_PTR ((long)ch + ch->size)


# define IS_PREV_FREE(ch, max) ch->size < max && ch->prev && !ch->prev->in_use // a verifier

# define IS_NEXT_FREE(ch, max) (ch->size < max && ch->next_size && (NEXT_CHUNK(ch))->in_use == false)

/*
** INCLUDES
*/

# include <stdlib.h>
# include <stdbool.h>
# include <sys/mman.h>
# include <sys/resource.h>

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
** - t_segment:	Represente un segment de memoire realiser a l'aide de mmap()
**				comprenant: les tinies, les smalls et les larges
** - t_chunk: Un troncon de memoire comprenant: Metadata + data
*/
typedef struct s_private_memory		t_private_memory;
typedef struct s_heap				t_heap;
typedef struct s_segment			t_segment;
typedef struct s_chunk				t_chunk;
typedef struct s_op					t_op;


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

struct	s_segment
{
	t_segment	*next;
	union
	{
		size_t		available_space; // For tiny and small
		size_t		seg_size; //For large chunks
	} u_u;
};

struct	s_heap
{
	t_segment			*tiny_segment;
	t_segment			*small_segment;
	t_segment			*large_segment;
};

struct s_op
{
	size_t		max_chunk_size;
	size_t		segment_size;
	size_t		offset:56;
	bool		is_large;
};

/*
*****************GLOBAL_VARS****************
*/

extern t_heap	g_heap;
extern t_op		g_op[4];
/*
 * Corbeilles utilisé pour stocker l'addresse des malloc free pour les
 * reutiliser sans parcourir l'ensemble d'un segment
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
void	free_segment(t_segment **head, t_segment *segment, t_op g_op);
bool	defrag(t_segment *segment, t_chunk *chunk, t_op g_op);
void	update_bins(t_segment *segment);
t_chunk *pop_from_bin(t_chunk *chunk, bool defrag);



#endif
