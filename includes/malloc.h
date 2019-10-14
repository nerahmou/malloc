/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   malloc.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/14 10:24:51 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/14 12:55:47 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef MALLOC
#define MALLOC
#define PROT_OPTS (PROT_READ | PROT_WRITE)
#define MAP_OPTS (MAP_ANON | MAP_PRIVATE)

#include <sys/mman.h>
#include "libft.h"

	

	void show_alloc_mem();
/*
	*****************MALLOC****************
	 */

	void *malloc(size_t size);

/*
	*****************REALLOC****************
	 */
	void *realloc(void *ptr, size_t size);


/*
	*****************FREE****************
	 */
void free(void *ptr);


#endif
