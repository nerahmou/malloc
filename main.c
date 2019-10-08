/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <nerahmou@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/08 09:40:14 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2019/10/08 16:15:07 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "test.h"
#include <stdio.h>

int main(int argc, const char *argv[])
{
	while(argc--)
	{
		printf("\n%s\n", *argv);
		argv++;
	}
	
	salut();

	return 0;
}
