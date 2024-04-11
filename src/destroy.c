/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:14:11 by sephilip          #+#    #+#             */
/*   Updated: 2024/04/11 12:14:11 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	exitmap(char **map, int ret, char *msg)
{
	ft_printf("%s", msg);
	free_map(map, ret);
}

void	free_map(char **map, int ret)
{
	int	i;

	i = 0;
	if (!map)
		return ;	
	while (map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
	exit(ret);
}

void	destroy_assets(t_mlx *root, t_image *asset)
{
	int	i;

	i = 0;
	while (i < 128)
	{
		if (asset[i].path && asset[i].img)
			mlx_destroy_image(root->mlx, asset[i].img);
		i++;
	}
}

void	killmlx(t_mlx *root, int all, int ret)
{
	destroy_assets(root, root->asset);
	if (root->asset)
		free(root->asset);
	if (all > 0)
	{
		mlx_destroy_image(root->mlx, root->frame.img);
		if (all > 1)
			mlx_destroy_window(root->mlx, root->window);
	}
	mlx_destroy_display(root->mlx);
	free(root->mlx);
	free_map(root->map->map, ret);
}

int	exit_game(t_mlx *root)
{
	destroy_assets(root, root->asset);
	if (root->asset)
		free(root->asset);
	mlx_destroy_image(root->mlx, root->frame.img);
	mlx_destroy_window(root->mlx, root->window);
	mlx_destroy_display(root->mlx);
	free(root->mlx);
	free_map(root->map->map, 0);
	return (0);
}
