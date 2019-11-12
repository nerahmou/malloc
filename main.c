/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 11:24:43 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/12 17:41:26 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */


#include "malloc.h"
#include "free.h"
#include <stdio.h>

int main(int argc, char* argv[]){
	//show_alloc_mem();
	void *tab[] = {malloc(0),
		malloc(16),
		malloc(16),
		malloc(16),
		malloc(16),
		malloc(16),
		malloc(18),
		malloc(511), // Meme resultat
		malloc(512), // mais pas meme categorie bizarre ???
		malloc(4095),
		malloc(4096),
		malloc(123456789)};
	//free(tab[1]);
	//free(tab[0]);
	//show_alloc_mem();
	show_alloc_mem();
	free(tab[1]);
	show_bins(TINY_BINS_LIMIT);
	free(tab[3]);
	show_bins(TINY_BINS_LIMIT);
	free(tab[5]);
	show_bins(TINY_BINS_LIMIT);
	free(tab[2]);
	show_bins(TINY_BINS_LIMIT);
	free(tab[6]);
	show_bins(TINY_BINS_LIMIT);
	free(tab[4]);
	show_bins(TINY_BINS_LIMIT);
	free(tab[7]);
	show_bins(TINY_BINS_LIMIT);
	show_alloc_mem();
	free(tab[8]);
	show_alloc_mem();
	/*free(tab[9]);
	show_alloc_mem();
	free(tab[10]);
	show_alloc_mem();
	free(tab[11]);
	show_alloc_mem();
	*///free(tab[3]);
	//show_alloc_mem();
	//free(tab[3]);
	//printf("%d", TINY_MAX_SIZE);
	//show_alloc_mem();
	return 1;
}
