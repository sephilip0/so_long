#include "../includes/so_long.h"

//change to mine prinft
void	exitmap(char **map, int ret, char *msg)
{
	printf("%s", msg);
	free_map(map, ret);
}

void	map_count_row(t_map *map, char *file)
{
	int		fd;
	int		i;
	char	*line;

	i = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		write(2, "ERROR OPEN FILE\n", 16);
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

void	free_map(char **map, int ret)
{
	int	i;

	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
	exit(ret);
}

void	init_map(t_map *map, char *file)
{
	int	fd;
	int	i;

	printf("INIT_MAP...\n");
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		write(2, "ERROR OPEN FILE 2\n", 18);
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

void	character_analyser(t_map *map, int i, int j)
{
	if (map->map[0][j] != '1' || map->map[map->height - 1][j] != '1')
		exitmap(map->map, 0, "WRONG MAP1\n");
	if (map->map[i][map->width - 1] != '1' || map->map[i][0] != '1')
		exitmap(map->map, 0, "WRONG MAP2\n");
	if (map->map[i][j] == 'C')
		(map->collect)++;
	if (map->map[i][j] == 'E')
	{
		map->exit_y = i;
		map->exit_x = j;
	}
	if (map->map[i][j] == 'P')
	{
		map->player_y = i;
		map->player_x = j;
	}
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
		exitmap(map->map, 0, "WRONG MAP3\n");
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
		width = ft_strlen(map->map[i]) - 1;
		if (width != map->width)
			exitmap(map->map, 0, "NOT RECTANGLE");
		j = 0;
		while (j < map->width)
		{
			c = map->map[i][j];
			if (!(c == '0' || c == '1' || c == 'C' || c == 'E' || c == 'P'))
				exitmap(map->map, 0, "ERROR LETTER");
			j++;
		}
		i++;
	}
}

void	name_check(char *name)
{
	int	i;
	int	ret;

	i = ft_strlen(name);
	ret = ft_strncmp(&name[i - 4], ".ber", 4);
	if (name[i - 5] == '/')
		ret = 1;
	if (ret)
	{
		write(2, "ERROR NAME\n", 11);
		exit (0);
	}
}

void	maptozero(t_map *map)
{
	map->height = 0;
	map->width = 0;
	map->collect = 0;
	map->player_x = 0;
	map->player_y = 0;
	map->exit_x = 0;
	map->exit_y = 0;
	map->active_exit = 0;
	map->idle = 0;
	map->walk = 1;
}

void	ft_putmat(char **mat)
{
	int	i;
	int	j;

	i = 0;
	while (mat[i])
	{
		j = 0;
		while (mat[i][j])
		{
			write(1, &mat[i][j], 1);
			j++;
		}
		i++;
	}
}

//AFTER SETITNG MAP->PLAYER_X AND Y REMOVE P FROM MAP
void	map_constructor(t_map *map, char *file)
{
	int		col;

	maptozero(map);
	name_check(file);
	map_count_row(map, file);
	if (map->height < 3)
	{
		write(2, "LESS THAN 3 LINES MAP\n", 22);
		exit(0);
	}
	init_map(map, file);
	check_map(map);
	map_param(map);
	col = 0;
	map->map[map->player_y][map->player_x] = '0';
	flood_fill(map, map->player_y, map->player_x, &col);
	if (!(col == map->collect && map->active_exit))
		exitmap(map->map, 0, "FLOOD FILL ERROR");
	map->map[map->player_y][map->player_x] = 'o';
}
