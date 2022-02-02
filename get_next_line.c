/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmesnard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 17:50:29 by hmesnard          #+#    #+#             */
/*   Updated: 2021/02/20 18:36:09 by hmesnard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_line(const char *s)
{
	char	*res;
	int		i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (!(res = malloc((i + 1) * sizeof(char))))
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '\n')
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*get_save(char *s)
{
	char	*res;
	int		reslen;
	int		i;
	int		j;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	reslen = (s[i]) ? ft_strlen(s + i + 1) : 0;
	if (!(res = malloc((reslen + 1) * sizeof(char))))
		return (NULL);
	j = 0;
	if (s[i])
		i++;
	while (s[i])
		res[j++] = s[i++];
	res[j] = '\0';
	free(s);
	return (res);
}

int		free_end(int return_value, char **line, char **save, char **buf)
{
	if (line)
	{
		free(*line);
		*line = NULL;
	}
	if (save)
	{
		free(*save);
		*save = NULL;
	}
	if (buf)
	{
		free(*buf);
		*buf = NULL;
	}
	return (return_value);
}

int		get_next_line(int fd, char **line)
{
	static char	*save[OPEN_MAX];
	char		*buf;
	int			readret;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE < 1)
		return (-1);
	if (!line || !(buf = malloc((BUFFER_SIZE + 1) * sizeof(char))))
		return (free_end(-1, NULL, &save[fd], NULL));
	readret = 1;
	while (!nl(save[fd]) && readret)
	{
		if ((readret = read(fd, buf, BUFFER_SIZE)) == -1)
			return (free_end(-1, NULL, &save[fd], &buf));
		buf[readret] = '\0';
		if (!(save[fd] = strjoin_free(save[fd], buf)))
			return (free_end(-1, NULL, &save[fd], &buf));
	}
	free(buf);
	if (!(*line = get_line(save[fd])))
		return (free_end(-1, NULL, &save[fd], NULL));
	if (!(save[fd] = get_save(save[fd])))
		return (free_end(-1, line, &save[fd], NULL));
	if (!readret)
		return (free_end(0, NULL, &save[fd], NULL));
	return (1);
}
