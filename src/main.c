/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:14:17 by sephilip          #+#    #+#             */
/*   Updated: 2024/04/11 12:14:17 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	maptozero(t_map *map)
{
	map->height = 0;
	map->width = 0;
	map->collect = 0;
	map->p_x = 0;
	map->p_y = 0;
	map->exit_x = 0;
	map->exit_y = 0;
	map->active_exit = 0;
	map->idle = 0;
	map->walk = 1;
}

int	main(int argc, char *argv[])
{
	t_mlx	root;
	t_image	frame;
	t_map	map;

	if (argc != 2)
	{
		write(2, "Error\nWrong number of arguments\n", 32);
		return (0);
	}
	map_constructor(&map, argv[1]);
	root_constructor(&root, &map, &frame);
	mlx_hook(root.window, 17, 1L << 0, exit_game, &root);
	mlx_key_hook(root.window, input_player, &root);
	mlx_loop_hook(root.mlx, looper, &root);
	mlx_loop(root.mlx);
	return (0);
}
