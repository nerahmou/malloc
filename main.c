/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <nerahmou@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/08 09:40:14 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/14 15:26:18 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "malloc.h"
#define MASIZE 4097

int main(int argc, char* argv[]){
	//char *un = (char*)mmap(NULL, 1, PROT_OPTS, MAP_OPTS, -1, 0);
	//char *deux = (char*)mmap(NULL, 1, PROT_OPTS, MAP_OPTS, -1, 0);
	
	char *deux = (char*)malloc(17);
	char *un = (char*)malloc(90);
	
	printf("%p\n%p\n", un, deux);
	free(un);
	free(deux);
	return 0;
}
