/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/04 15:26:56 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/06 17:41:06 by nerahmou    ###    #+. /#+    ###.fr     */
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

# define MUNMAP_FAILED -1

# define PAGE_SIZE getpagesize

# define TINY_MAX_SIZE 1024
# define SMALL_MAX_SIZE 8192

# define TINY_REGION_SIZE 2097152
# define SMALL_REGION_SIZE 16777216
# define LARGE_REGION_SIZE 80

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
