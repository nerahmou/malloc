/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/04 15:26:56 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/04 15:56:46 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef MALLOC_H

/*
******************DEFINES****************
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

# define TINY_REGION_SIZE (size_t)PAGE_SIZE * 512
# define SMALL_REGION_SIZE (size_t)PAGE_SIZE * 4096
# define LARGE_REGION_SIZE (REG_HEAD_SIZE + CHUNK_HEAD_SIZE)

# define TINY_REGION_HEAD &(g_heap.tiny_region)
# define SMALL_REGION_HEAD &(g_heap.small_region)
# define LARGE_REGION_HEAD &(g_heap.large_region)

# define NB_TYPE_OF_REGION 3

# define ALIGNMENT 16

/*
******************INCLUDES****************
*/
# include <stdbool.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <unistd.h>
# include "libft.h"

/*
******************TYPEDEFS****************
*/
typedef struct s_heap	t_heap;
typedef struct s_region	t_region;
typedef struct s_chunk	t_chunk;

/*
******************STRUCTS****************
*/

/*
** -void *data	is the address returned by malloc().
**				If the chunk is freed, *data is used like a next pointer and
**				point to the previous freed element of the same bin.
*/
struct			s_chunk
{
	t_chunk		*prev;
	t_chunk		*next;
	size_t		size;
	size_t		in_use;
	void		*data;
};

/*
** -unsigned max_chunk_size	 represents the limit size of a chunk in the region
*/
struct			s_region
{
	t_region	**head;
	t_region	*next;
	size_t		size;
	unsigned	space;
	unsigned	max_chunk_size;
};

struct			s_heap
{
	t_region	*tiny_region;
	t_region	*small_region;
	t_region	*large_region;
};

/*
*****************GLOBALS****************
*/
extern t_heap	g_heap;

/*
******************PROTOTYPES****************
*/
void			*malloc(size_t size);
void			*realloc(void *ptr, size_t size);
void			show_alloc_mem(void);
size_t			required_size(size_t size, size_t header_size, size_t mult);
t_region		*is_valid_ptr(void *ptr);
t_region		*get_region_to_place_chunk(size_t size);
#endif
