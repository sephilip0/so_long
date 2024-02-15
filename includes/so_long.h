#ifndef SO_LONG_H
# define SO_LONG_H

#include "./libft/libft.h"
#include "../mlx/mlx.h"
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define ASSET_NBR	128
#define SCALER		2

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
	int			player_x;
	int			player_y;
	int			player_state_x;
	int			player_state_y;
	int			movement_count;
	int			current_asset;
	t_image		front;
	t_image		frame;
}	t_frame;

typedef struct s_map
{
	char	**map;
//	char	**collisions;
//	char	*collect_str;
	int		collect;
	int		height;
	int		width;
	int		player_y;
	int		player_x;
	int		idle;
	int		active_exit;
//	int		player_state_x;
//	int		player_state_y;
	int		exit_y;
	int		exit_x;
}	t_map;

typedef struct s_mlx
{
	void		*mlx;
	void		*window;
//	t_frame		*frame;
	t_map		*map;
//mine
	t_image		frame;
	t_image		*asset;
	int		dif_timer;
	long		frame_time;
	int			steps;
	int			step_bol;
	char		*steps_str;
	int			s;
	int			mov;
	int			x;
	int			y;
}	t_mlx;

//MAIN.C
void	map_count_row(t_map *map, char *file);
void	free_map(char **map);
void	init_map(t_map *map, char *file);
void	flood_fill(t_map *map, int y, int x, int *c);
int	map_param(t_map *map);
void	check_map(t_map *map);
void	map_constructor(t_map *map, char *file);
void	ft_putmat(char **mat);

//TEST.C
void	set_path(t_image *asset);
void	init_img(t_image *asset, t_mlx	*root);
t_image	*init_assets(t_mlx *root);
unsigned int	get_pixel_color(t_image *img, int x, int y);
void	put_pixel(t_image *img, int x, int y, int color);
void	draw_part(t_mlx *root, t_image *image, int x, int y);
void	draw_map(t_mlx *root);
void	movement_player(t_mlx *root, int movx, int movy);
int	input_player(int keysym, t_mlx *root);
int	looper(t_mlx *root);



#endif /*SO_LONG_H */
