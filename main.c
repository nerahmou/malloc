/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 11:24:43 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/18 14:13:00 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */


#include "malloc.h"
#include "free.h"
#include <stdio.h>

int main(int argc, char* argv[]){
	
	unsigned char d = -1;
	void *tab[255];
	short	i;

/*	i = -1;
	while (++i < 255)
		tab[i] = malloc(i + 96);
	show_alloc_mem();
	i = -1;
	while (++i < 255)
	{
		free(tab[i]);
		show_bins();
	}
	show_alloc_mem();
*/
	void *a = malloc(16);
	void *b = malloc(16);
	void *c = malloc(16);
	free(b);
	show_bins();
	free(a);
	show_bins();
	show_alloc_mem();
	/*void *tab[255] = {

		malloc(16),
		malloc(26),
		malloc(16),
		malloc(36),
		malloc(16),
		malloc(59),
		malloc(159),
		malloc(490),
		malloc(512),
		malloc(1159),
		malloc(1159),
		malloc(4081),
		malloc(4096),
		malloc(5159)
	};*/

	return 1;
}
