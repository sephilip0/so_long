/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:13:40 by sephilip          #+#    #+#             */
/*   Updated: 2024/04/11 12:13:41 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	set_path(t_image *asset)
{
	asset[1].path = "./assets/diamond_rush/idle1.xpm";
	asset[2].path = "./assets/diamond_rush/idle2.xpm";
	asset[3].path = "./assets/diamond_rush/walkr1.xpm";
	asset[4].path = "./assets/diamond_rush/walkr2.xpm";
	asset[5].path = "./assets/diamond_rush/walkl1.xpm";
	asset[6].path = "./assets/diamond_rush/walkl2.xpm";
	asset[48].path = "./assets/diamond_rush/base.xpm";
	asset[49].path = "./assets/diamond_rush/wall.xpm";
	asset[50].path = "./assets/diamond_rush/rock.xpm";
	asset[69].path = "./assets/diamond_rush/exit.xpm";
	asset[99].path = "./assets/diamond_rush/diamond.xpm";
	asset[111].path = "./assets/diamond_rush/base.xpm";
}

void	init_img(t_image *asset, t_mlx	*root)
{
	int	i;

	i = 0;
	while (i < ASSET_NBR)
	{
		if (asset[i].path)
		{
			asset[i].img = mlx_xpm_file_to_image(root->mlx, \
				asset[i].path, &asset[i].width, &asset[i].height); 
			if (!asset[i].img)
			{
				destroy_assets(root, asset);
				mlx_destroy_display(root->mlx);
				free(root->mlx);
				free_map(root->map->map, 1);
			}
			asset[i].addr = mlx_get_data_addr(asset[i].img, &asset[i].bpp, \
					&asset[i].line_length, &asset[i].endian);
		}
		i++;
	}
}

t_image	*init_assets(t_mlx *root)
{
	int			i;
	t_image		*asset;

	asset = malloc(sizeof(t_image) * ASSET_NBR);
	if (!asset)
	{
		mlx_destroy_display(root->mlx);
		free(root->mlx);
		exitmap(root->map->map, 1, "Error\nCOuld load assets\n");
	}
	i = 0;
	while (i < ASSET_NBR)
	{
		asset[i].path = NULL;
		i++;
	}
	set_path(asset);
	init_img(asset, root);
	return (asset);
}

void	draw_part(t_mlx *root, t_image *image, int x, int y)
{
	int	cur_x;
	int	cur_y;
	int	counter;

	counter = 0;
	cur_y = 0;
	while (cur_y < root->s)
	{
		cur_x = 0;
		while (cur_x < root->s)
		{
			put_pixel(&root->frame, x + cur_x, y + cur_y, \
			get_pixel_color(image, (cur_x / SCALER), (cur_y / SCALER)));
			cur_x++;
			counter++;
		}
		cur_y++;
	}
	mlx_put_image_to_window(root->mlx, root->window, root->frame.img, 0, 0);
}

void	draw_map(t_mlx *root, int ch)
{
	int	cur_x;
	int	cur_y;
	int	i;
	int	j;
	int	counter;

	counter = 0;
	cur_y = 0;
	while (cur_y < root->frame.height)
	{
		cur_x = 0;
		while (cur_x < root->frame.width)
		{
			i = cur_x / root->s;
			j = cur_y / root->s;
			ch = root->map->map[j][i];
			put_pixel(&root->frame, cur_x, cur_y, \
			get_pixel_color(&root->asset[ch], \
			(cur_x % root->s / SCALER), (cur_y % root->s / SCALER)));
			cur_x++;
			counter++;
		}
		cur_y++;
	}
	mlx_put_image_to_window(root->mlx, root->window, root->frame.img, 0, 0);
}
