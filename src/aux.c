/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:13:48 by sephilip          #+#    #+#             */
/*   Updated: 2024/04/11 12:13:48 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

unsigned int	get_pixel_color(t_image *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	return (*(unsigned int *)dst);
}

void	put_pixel(t_image *img, int x, int y, int color)
{
	char		*dst;

	if (color == 16253176)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
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
		write(2, "Error\nWrong name\n", 17);
		exit (0);
	}
}

void	character_analyser(t_map *map, int i, int j)
{
	if (map->map[0][j] != '1' || map->map[map->height - 1][j] != '1')
		exitmap(map->map, 0, "Error\nWrong map\n");
	if (map->map[i][map->width - 1] != '1' || map->map[i][0] != '1')
		exitmap(map->map, 0, "Error\nWrong map\n");
	if (map->map[i][j] == 'C')
		(map->collect)++;
	if (map->map[i][j] == 'E')
	{
		map->exit_y = i;
		map->exit_x = j;
	}
	if (map->map[i][j] == 'P')
	{
		map->p_y = i;
		map->p_x = j;
	}
}
