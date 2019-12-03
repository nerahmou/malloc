/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 11:24:43 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/12/03 18:55:13 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */


#include "free.h"
#include "malloc.h"

int main(int argc, char* argv[]){
	
	void	*tab[] = {
		malloc(16),
		malloc(36),
		malloc(56),
		malloc(5623),
		malloc(565546),
		malloc(56),
		malloc(16)
	};
	show_alloc_mem();
	free(tab[0]);
	free(tab[2]);
	free(tab[1]);
	free(tab[5]);
	free(tab[6]);
	free(tab[3]);
	free(tab[4]);
	show_bins(20);
	show_alloc_mem();
	/*free(tab[4]);
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
