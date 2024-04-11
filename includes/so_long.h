/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:15:29 by sephilip          #+#    #+#             */
/*   Updated: 2024/04/11 12:16:22 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "./libft/libft.h"
# include "../mlx/mlx.h"
# include <X11/keysym.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include <time.h>

//#define malloc(x) NULL
# define STEPS_ON_SCREEN	1
# define ASSET_NBR	128
# define SCALER		3

typedef struct s_image
{
	void	*img;
	char	*addr;
	char	*path;
	int		bpp;
	int		line_length;
	int		endian;
	int		height;
	int		width;
}	t_image;

typedef struct s_frame
{
	t_image		back;
	t_image		player;
	int			p_x;
	int			p_y;
	int			player_state_x;
	int			player_state_y;
	t_image		front;
	t_image		frame;
}	t_frame;

typedef struct s_map
{
	char	**map;
	int		collect;
	int		height;
	int		width;
	int		p_y;
	int		p_x;
	int		idle;
	int		walk;
	int		active_exit;
	int		exit_y;
	int		exit_x;
}	t_map;

typedef struct s_mlx
{
	void		*mlx;
	void		*window;
	t_map		*map;
	t_image		frame;
	t_image		*asset;
	int			dif_timer;
	int			steps;
	int			s;
}	t_mlx;

//ASSETS.C
void			set_path(t_image *asset);
void			init_img(t_image *asset, t_mlx	*root);
t_image			*init_assets(t_mlx *root);
void			draw_part(t_mlx *root, t_image *image, int x, int y);
void			draw_map(t_mlx *root, int ch);

//AUX.C
unsigned int	get_pixel_color(t_image *img, int x, int y);
void			put_pixel(t_image *img, int x, int y, int color);
void			name_check(char *name);
void			ft_putmat(char **mat);
void			character_analyser(t_map *map, int i, int j);

//BEHINDSCREEN.C
void			movement_player(t_mlx *root, int movx, int movy, int lr);
int				input_player(int keysym, t_mlx *root);
int				switcher(int base, int min, int max);
int				looper(t_mlx *root);
void			print_steps(t_mlx *root);

//CONSTRUCTOR.C
void			root_constructor(t_mlx *root, t_map *map, t_image *frame);
void			map_constructor(t_map *map, char *file);

//DESTROY.C
void			exitmap(char **map, int ret, char *msg);
void			free_map(char **map, int ret);
void			destroy_assets(t_mlx *root, t_image *asset);
void			killmlx(t_mlx *root, int all, int ret);
int				exit_game(t_mlx *root);

//MAIN.C
void			maptozero(t_map *map);

//MAP.C
void			map_count_row(t_map *map, char *file);
void			init_map(t_map *map, char *file);
void			flood_fill(t_map *map, int y, int x, int *c);
void			map_param(t_map *map);
void			check_map(t_map *map);

#endif /*SO_LONG_H */
