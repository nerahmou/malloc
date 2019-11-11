/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/22 11:24:43 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/11 17:19:53 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */


#include "malloc.h"
#include "free.h"
#include <stdio.h>

int main(int argc, char* argv[]){
	void *tab[] = {malloc(0), malloc(10), malloc(15), malloc(511),
		malloc(512), malloc(4095), malloc(4096), malloc(123456789)};
	return 1;
}
