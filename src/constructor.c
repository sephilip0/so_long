/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:14:05 by sephilip          #+#    #+#             */
/*   Updated: 2024/04/11 12:14:05 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	root_constructor(t_mlx *root, t_map *map, t_image *frame)
{
	root->map = map;
	root->mlx = mlx_init();
	if (!root->mlx)
		exitmap(root->map->map, 1, "Error\nCouldnt init mlx\n");
	root->asset = init_assets(root);
	root->s = 24 * SCALER;
	frame->height = root->map->height * root->s;
	frame->width = root->map->width * root->s;
	frame->img = mlx_new_image(root->mlx, frame->width, frame->height);
	if (!frame->img)
		killmlx(root, 0, 1);
	frame->addr = mlx_get_data_addr(frame->img, &frame->bpp, \
	&frame->line_length, &frame->endian);
	root->frame = *frame;
	root->window = mlx_new_window(root->mlx, frame->width, \
	frame->height, "so_long");
	if (!root->window)
		killmlx(root, 1, 1);
	root->steps = 0;
	root->dif_timer = 0;
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
		write(2, "Error\nMap is too low\n", 21);
		exit(0);
	}
	init_map(map, file);
	check_map(map);
	map_param(map);
	col = 0;
	map->map[map->p_y][map->p_x] = '0';
	flood_fill(map, map->p_y, map->p_x, &col);
	if (!(col == map->collect && map->active_exit))
		exitmap(map->map, 0, "Error\nNot winnable game\n");
	map->map[map->p_y][map->p_x] = 'o';
}
