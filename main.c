/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 11:24:43 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/25 11:15:48 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */


#include "malloc.h"
#include "free.h"
#include <stdio.h>

int main(int argc, char* argv[]){
	
	
	if (argv[1])
		debug = 1;
	void *tab[255] = {
		malloc(16),
		malloc(26),
		malloc(16),
		malloc(36),
		malloc(16),
		malloc(59),
		malloc(159),
		malloc(490),
		malloc(512),
		malloc(992),
		malloc(4081),
		malloc(4096),
		malloc(5159),
		malloc(8160),
	};
	show_alloc_mem();
	/*free(tab[0]);
	show_alloc_mem();
	free(tab[0]);
	show_alloc_mem();
	free(tab[2]);
	show_alloc_mem();
	free(tab[4]);
	show_alloc_mem();
	free(tab[6]);
	show_alloc_mem();
	free(tab[8]);
	show_alloc_mem();
	free(tab[10]);
	show_alloc_mem();
	free(tab[12]);
	show_alloc_mem();
	free(tab[1]);
	show_alloc_mem();
	free(tab[3]);
	show_alloc_mem();
	free(tab[5]);
	show_alloc_mem();
	free(tab[7]);
	show_alloc_mem();
	free(tab[9]);
	show_alloc_mem();
	free(tab[11]);
	show_alloc_mem();
	*/
	return 1;
}
