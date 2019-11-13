/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 11:24:43 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/13 16:23:53 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */


#include "malloc.h"
#include "free.h"
#include <stdio.h>

int main(int argc, char* argv[]){
	//show_alloc_mem();
//	show_bins();
	void *tab[] = {
		malloc(1), //0
		malloc(15),//1
		malloc(16), //2
		malloc(200),//3
		malloc(300), //4
		malloc(1081),//5
		malloc(2080),//6
	
		malloc(4081),//7
		malloc(4096),//8
		malloc(123456789)//9
	};
	show_alloc_mem();
	free(tab[0]);
	free(tab[2]);
	free(tab[4]);
	free(tab[6]);
	free(tab[1]);
	free(tab[3]);
	free(tab[5]);
	
	free(tab[5]);
	free(tab[6]);
	free(tab[7]);
	free(tab[9]);
	free(tab[8]);
	//show_alloc_mem();
	
//	show_bins();
	//show_alloc_mem();
	/*free(tab[0]);
	free(tab[1]);
	show_alloc_mem();
	*/return 1;
}
