//#include "../includes/so_long.h"

#include "../includes/libft/libft.h"
#include "../includes/get_next_line/get_next_line.h"
//DELETE, its in SO_LONG.H
typedef struct s_map
{
	char	**map;
	char	**collisions;
	char	*collect_str;
	int		collect;
	int		height;
	int		width;
	int		player_y;
	int		player_x;
	int		exit_y;
	int		exit_x;
	int		active_exit;
}	t_map;


void	map_count_row(t_map *map, char *file)
{
	int	fd;
	int	i;
	char	*line;

	i = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		write(2, "ERROR OPEN FILE\n", 16);
		exit(0);
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

void	free_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
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
		exit(0);
	}
	map->map = (char **)ft_calloc((map->height + 1), sizeof(char *));
	if (!map->map)
		exit(0);	
	i = 0;
	while (i < map->height)
	{
		map->map[i] = get_next_line(fd);
		i++;
	}
	map->width = ft_strlen(map->map[0]) - 1;
	close(fd);
}

void	flood_fill(t_map *map, int y, int x, int *c)
{
	//0 -> o
	//C -> c
	printf("y: %d x: %d\n", y , x);
	if (map->map[y][x] == '0')
		map->map[y][x] = 'o';
	if (map->map[y][x] == 'E')
		map->active_exit = 1;
	if (map->map[y][x] == 'C')
	{
		(*c)++;
		map->map[y][x] = 'c';
	}
	if ((map->map[y + 1][x] != '1' && map->map[y + 1][x] != 'o' && map->map[y + 1][x] != 'c'))
		flood_fill(map, y + 1, x, c);
	if ((map->map[y - 1][x] != '1' && map->map[y - 1][x] != 'o' && map->map[y - 1][x] != 'c'))
		flood_fill(map, y - 1, x, c);
	if ((map->map[y][x + 1] != '1' && map->map[y][x + 1] != 'o' && map->map[y][x + 1] != 'c'))
		flood_fill(map, y, x + 1, c);
	if ((map->map[y][x - 1] != '1' && map->map[y][x - 1] != 'o' && map->map[y][x - 1] != 'c'))
		flood_fill(map, y, x - 1, c);
}

int	map_param(t_map *map)
{
	int	i;
	int	j;
	int	exit;
	int	player;

	printf("MAP_PARAM...\n");
	exit = 0;
	player = 0;
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (map->map[0][j] != '1')
			{
				printf("[%d][%d]: 1\n", 0, j);
				return (1);
			}
			if (map->map[i][0] != '1')
			{
				printf("[%d][%d]: 2\n", i, 0);
				return (1);
			}
       			if (map->map[i][map->width - 1] != '1')
			{
				printf("[%d][%d]: 3\n", i, (map->width - 1));
				return (1);
			}
			if (map->map[map->height - 1][j] != '1')
			{
				printf("[%d][%d]: 4\n", (map->height - 1), j);
				return (1);
			}
			if (map->map[i][j] == 'C')
				(map->collect)++;
			if (map->map[i][j] == 'E')
			{
				map->exit_y = i;
				map->exit_x = j;
				exit++;
			}
			if (map->map[i][j] == 'P')
			{
				map->player_y = i;
				map->player_x = j;
				player++;
			}
			j++;
		}
		i++;
	}
	printf("col: %d\n", map->collect);
	printf("exit: %d\n", exit);
	printf("player: %d\n", player);
	if ((map->collect < 1) || (player != 1) || (exit != 1))
		return (1);
	return (0);
}

void	check_map(t_map *map)
{
	int	width;
	int	i;
	int	j;
	int	c;

	printf("CHECK_MAP...\n");
	printf("map_height: %d\n", map->height);
	printf("map_width: %d\n", map->width);
	if (map->height < 3)
	{
		write(2, "LESS THAN 3 LINES MAP\n", 22);
		exit(0);
	}
	i = 0;
	while (i < map->height)
	{
		width = ft_strlen(map->map[i]) - 1;
		if (width != map->width)
		{
			write(2, "NOT RECTANGLE\n", 14);
			free_map(map->map);
			exit(0);
		}
		j = 0;
		while (j < map->width)
		{
			c = map->map[i][j];
			if (!(c == '0' || c == '1' || c == 'C' || c == 'E' || c == 'P'))
			{
				write(2, "ERROR LETTER\n", 13);
				free_map(map->map);
				exit(0);
			}
			j++;
		}
		i++;
	}
}

void	map_constructor(t_map *map, char *file)
{
	int	i;
	//checkname
	printf("MAP_CONSTRUCTOR...\n");
	map->collect = 0;
	map->player_x = 0;
	map->player_y = 0;
	map->exit_x = 0;
	map->exit_y = 0;
	map->active_exit = 0;
	map_count_row(map, file);
	init_map(map, file);
	check_map(map);
	i = 0;
	i += map_param(map);
	if (i != 0)
	{
		free_map(map->map);
		exit(0);
	}
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

int	main(int argc, char *argv[])
{
	t_map	map;
	int	col;

	col = 0;
	if (argc != 2)
	{
		printf("NOT THE RIGHT ARGUMENTS");
		return (0);	
	}
	map_constructor(&map, argv[1]);
	ft_putmat(map.map);
	flood_fill(&map, map.player_y, map.player_x, &col);
	if (col == map.collect && map.active_exit)
		printf("YES! YOU CAN PLAY THE GAME\n");
	else
		printf("NO! UNPLAYABLE GAME\n");
	ft_putmat(map.map);
	free_map(map.map);
	return (0);
}
