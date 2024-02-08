/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:41:57 by sephilip          #+#    #+#             */
/*   Updated: 2023/11/02 13:27:24 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*str;
	int			a;
	int			size;

	str = NULL;
	if (BUFFER_SIZE <= 0 || fd < 0 || (read(fd, buf, 0) < 0))
		return (ft_clear(&buf, str, -1));
	if (*buf)
	{
		size = ft_verline(buf) + 1;
		str = ft_inccpy(str, buf, size);
	}
	while (ft_verline(str) == -1)
	{
		a = read(fd, buf, BUFFER_SIZE);
		if (a <= 0)
			return (ft_clear(&buf, str, a));
		buf[a] = 0;
		size = ft_verline(buf) + 1;
		str = ft_inccpy(str, buf, size);
	}
	if (*buf)
		ft_strlcpy(buf, &buf[size], BUFFER_SIZE);
	return (str);
}

/*
int	main()
{
	int	fd;
	char	*ptr;

	fd = open("41_no_nl", O_RDONLY);
	ptr = get_next_line(fd);
	printf("--->%s!", ptr);
	free(ptr);
	ptr = get_next_line(fd);
	printf("--->%s!", ptr);
	free(ptr);
	ptr = get_next_line(fd);
	printf("--->%s!", ptr);
	free(ptr);
	ptr = get_next_line(fd);
	printf("--->%s!", ptr);
	free(ptr);
	ptr = get_next_line(fd);
	printf("--->%s", ptr);
	free(ptr);*
	ptr = get_next_line(fd);
	printf("--->%s", ptr);
	free(ptr);
	close(fd);
	return (0);
}*/
