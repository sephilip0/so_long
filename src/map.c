/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:14:24 by sephilip          #+#    #+#             */
/*   Updated: 2024/04/11 12:14:25 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	map_count_row(t_map *map, char *file)
{
	int		fd;
	int		i;
	char	*line;

	i = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Error\nCouldnt open file\n", 24);
		exit(1);
	}
	line = get_next_line(fd);
	while (line != NULL)
	{
		free(line);
		i++;
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	map->height = i;
}

void	init_map(t_map *map, char *file)
{
	int	fd;
	int	i;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Error\nCouldnt open file\n", 24);
		exit(1);
	}
	map->map = (char **)ft_calloc((map->height + 1), sizeof(char *));
	if (!map->map)
		exit(1);
	i = 0;
	while (i < map->height)
	{
		map->map[i] = get_next_line(fd);
		i++;
	}
	map->width = ft_strlen(map->map[0]) - 1;
	close(fd);
}

//0 -> o
//C -> c
void	flood_fill(t_map *map, int y, int x, int *c)
{
	if (map->map[y][x] == '0')
		map->map[y][x] = 'o';
	if (map->map[y][x] == 'E')
		map->active_exit = 1;
	if (map->map[y][x] == 'C')
	{
		(*c)++;
		map->map[y][x] = 'c';
	}
	if ((map->map[y + 1][x] != '1' && map->map[y + 1][x] != 'o' && \
		map->map[y + 1][x] != 'c'))
		flood_fill(map, y + 1, x, c);
	if ((map->map[y - 1][x] != '1' && map->map[y - 1][x] != 'o' && \
		map->map[y - 1][x] != 'c'))
		flood_fill(map, y - 1, x, c);
	if ((map->map[y][x + 1] != '1' && map->map[y][x + 1] != 'o' && \
		map->map[y][x + 1] != 'c'))
		flood_fill(map, y, x + 1, c);
	if ((map->map[y][x - 1] != '1' && map->map[y][x - 1] != 'o' && \
		map->map[y][x - 1] != 'c'))
		flood_fill(map, y, x - 1, c);
}

void	map_param(t_map *map)
{
	int	i;
	int	j;
	int	exiton;
	int	player;

	exiton = 0;
	player = 0;
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			character_analyser(map, i, j);
			if (map->map[i][j] == 'P')
				player++;
			if (map->map[i][j] == 'E')
				exiton++;
			j++;
		}
		i++;
	}
	if ((map->collect < 1) || (player != 1) || (exiton != 1))
		exitmap(map->map, 0, "Error\nWrong map\n");
}

void	check_map(t_map *map)
{
	int	width;
	int	i;
	int	j;
	int	c;

	i = 0;
	while (i < map->height)
	{
		if ((i == map->height -1 && map->map[i][map->width] != '\n'))
			width = ft_strlen(map->map[i]);
		else
			width = ft_strlen(map->map[i]) - 1;
		if (width != map->width)
			exitmap(map->map, 0, "Error\nNot rectangular\n");
		j = 0;
		while (j < map->width)
		{
			c = map->map[i][j];
			if (!(c == '0' || c == '1' || c == 'C' || c == 'E' || c == 'P'))
				exitmap(map->map, 0, "Error\nWrong letter\n");
			j++;
		}
		i++;
	}
}
