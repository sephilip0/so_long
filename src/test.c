#include "mlx.h"
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ASSET_NBR	1

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
	int		exit_y;
	int		exit_x;
}	t_map;

typedef struct s_mlx
{
	void		*mlx;
	void		*window;
	t_frame		*frame;
	t_map		*map;
	t_image		*asset;
	long		frame_time;
	int			steps;
	int			step_bol;
	char		*steps_str;
	int			s;
	int			mov;
	int			x;
	int			y;
}	t_mlx;

void	set_path(t_image *asset)
{
	asset[0].path = "./assets/idle1.xpm";
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
				printf("BOOM2\n");
//				free();
			}
			asset[i].addr = mlx_get_data_addr(asset[i].img, &asset[i].bpp, \
					&asset[i].line_length, &asset[i].endian);
		}
		i++;
	}
}

t_image	*init_assets(t_mlx *root)
{
	int	i;
	t_image		*asset;

	asset = malloc(sizeof(t_image) * ASSET_NBR);
	if (!asset)
	{
		printf("BOOM\n");
//		free()
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

int main()
{
	t_mlx	root;
	t_image	*asset;

	root.mlx = mlx_init();
	root.window = mlx_new_window(root.mlx, 1000, 1000, "duckgame");
//	duck.path = "./assets/idle1.xpm";
//	duck.img = mlx_new_image(root.mlx, 24, 27);
//	duck.img = mlx_xpm_file_to_image(root.mlx, duck.path, &duck.width, &duck.height);
//	duck.addr = mlx_get_data_addr(duck.img, &duck.bpp, &duck.line_length, &duck.endian);
	asset = init_assets(&root);
	mlx_put_image_to_window(root.mlx, root.window, asset[0].img, 500 - 12, 500 - 14);
	mlx_loop(root.mlx);
	return (0);
}
