#ifndef SO_LONG_H
# define SO_LONG_H

#include "./get_next_line/get_next_line.h"
#include "./libft/libft.h"
#include "mlx.h"
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
	char	**collisions;
	char	*collect_str;
	int		collect;
	int		height;
	int		width;
	int		player_y;
	int		player_x;
	int		idle;
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

#endif /*SO_LONG_H */
