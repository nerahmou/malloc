/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: nerahmou <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2017/11/29 18:09:11 by nerahmou     #+#   ##    ##    #+#       */
/*   Updated: 2018/09/11 10:41:19 by nerahmou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char		*ft_realloc(char **line, char *str)
{
	char *tmp;

	if (!*line)
	{
		if (!(*line = ft_strdup(str)))
			return (NULL);
	}
	else
	{
		if (!(tmp = ft_strjoin(*line, str)))
			return (NULL);
		ft_strdel(line);
		*line = tmp;
	}
	return (*line);
}

static int	ft_sub_tmp(char **line, char **tmp)
{
	char	*str;
	char	*apres_back_n;

	str = NULL;
	if (!**tmp)
		return (0);
	if ((apres_back_n = ft_strchr(*tmp, '\n')))
	{
		(*tmp)[ft_char_pos(*tmp, '\n')] = 0;
		if (!(ft_realloc(line, *tmp)))
			return (-1);
		if (apres_back_n[1] && !(str = ft_strdup(&apres_back_n[1])))
			return (-1);
		(*tmp)[ft_char_pos(*tmp, '\n')] = '\n';
		ft_strdel(tmp);
		*tmp = str;
		return (1);
	}
	if (!(ft_realloc(line, *tmp)))
		return (-1);
	ft_strdel(tmp);
	return (0);
}

static	int	ft_fill_line(char **line, char **tmp, int fd)
{
	char	buff[BUFF_SIZE + 1];
	char	*apres_back_n;
	int		ret;

	while ((ret = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = 0;
		if ((apres_back_n = ft_strchr(buff, '\n')))
		{
			buff[ft_char_pos(buff, '\n')] = 0;
			if (!(ft_realloc(line, buff)))
				return (-1);
			if (apres_back_n[1] && !(*tmp = ft_strdup(&apres_back_n[1])))
				return (-1);
			break ;
		}
		else if (!(ft_realloc(line, buff)))
			return (-1);
	}
	return (ret);
}

int			get_next_line(int fd, char **line)
{
	static char		*tmp = NULL;

	if (BUFF_SIZE < 1 || fd < 0 || !line)
		return (-1);
	*line = NULL;
	if (tmp && ft_sub_tmp(line, &tmp) == 1)
		return (1);
	if (ft_fill_line(line, &tmp, fd) == -1)
		return (-1);
	if (*line)
		return (1);
	return (0);
}
