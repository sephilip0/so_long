#include "mlx.h"
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define ASSET_NBR	2
#define SCALER		1

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
//	t_frame		*frame;
	t_map		*map;
//mine
	t_image		*frame;
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
	asset[0].path = "./assets/diamond_rush/wall.xpm";
	asset[1].path = "./assets/duck_idle2.xpm";
//	asset[1].path = "./assets/diamond_rush/bush.xpm";
	asset[2].path = "./assets/diamond_rush/base.xpm";
//	asset[0].path = "./assets/duck_idle1.xpm";
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

unsigned int	get_pixel_color(t_image *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_length  + x * (img->bpp / 8));
	return (*(unsigned int *)dst);
}

void	put_pixel(t_image *img, int x, int y, int color)
{
	char		*dst;

	if (color == 0)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_xy(t_mlx *root, t_image *asset, int x, int y)
{
	unsigned int	color;
	int	counter;
	int	cur_x;
	int	cur_y;

	cur_y = 0;
	while (cur_y < root->frame->height)
	{
		cur_x = 0;
		while (cur_x < root->frame->width)
		{
			color = get_pixel_color(asset, cur_x, cur_y);
			//if (color != rgb_to_int(0, 255, 255, 255))
			put_pixel(root->frame, cur_x, cur_y, color);
			cur_x++;
			counter++;
		}
		cur_y++;
	}
	printf("\n%d,%d\n", x, y);
	printf("counter: %d\n", counter);
//mlx_put_image_to_window(root->mlx, root->window, root->asset[0].img, x, y);
	mlx_put_image_to_window(root->mlx, root->window, root->frame->img, x, y);
}

void	draw(t_mlx *root, t_image *frame)
{
	unsigned int	color;
	int	cur_x;
	int	cur_y;

	cur_y = 0;
	while (cur_y < frame->height)
	{
		cur_x = 0;
		while (cur_x < frame->width)
		{
			color = get_pixel_color(frame, cur_x, cur_y);
			//if (color != rgb_to_int(0, 255, 255, 255))
			put_pixel(frame, cur_x, cur_y, color);
			cur_x++;
		}
		cur_y++;
	}
	mlx_put_image_to_window(root->mlx, root->window, frame->img, 0, 0);
}

t_image	*framing(t_mlx *root, int width, int height)
{
	t_image	*frame;

	frame = malloc(sizeof(t_image));
	if (!frame)
	{
		printf("BOOM\n");
		//free()
	}
	frame->img = mlx_new_image(root->mlx, 24 * SCALER, 24 * SCALER);
	if (!frame->img)
	{
		printf("BOOM2\n");
		//free()
	}
	frame->addr = mlx_get_data_addr(frame->img, &frame->bpp, &frame->line_length, &frame->endian);
	
	frame->height = height * SCALER;
	frame->width = width * SCALER;
	printf("frame_width: %d\n", frame->width);
	printf("frame_height: %d\n", frame->height);
	printf("frame_ll: %d\n", frame->line_length);
	return (frame);
}

int main()
{
	t_mlx	root;
//	t_image	*asset;

	root.mlx = mlx_init();
	root.window = mlx_new_window(root.mlx, 1000, 1000, "duckgame");
//	duck.path = "./assets/idle1.xpm";
//	duck.img = mlx_new_image(root.mlx, 24, 27);
//	duck.img = mlx_xpm_file_to_image(root.mlx, duck.path, &duck.width, &duck.height);
//	duck.addr = mlx_get_data_addr(duck.img, &duck.bpp, &duck.line_length, &duck.endian);
	root.asset = init_assets(&root);
	root.frame = framing(&root, root.asset->width, root.asset->height);
//	draw(&root, &root.asset[1]);
	draw_xy(&root, &root.asset[0], 200, 200);
	draw_xy(&root, &root.asset[1], 300, 300);
	draw_xy(&root, &root.asset[2], 500, 500);
//	draw(&root, &asset[1]);
//	mlx_put_image_to_window(root.mlx, root.window, asset[0].img, 500 - 12, 500 - 14);
//	sleep(1);
//	mlx_put_image_to_window(root.mlx, root.window, asset[1].img, 200 - 12, 200 - 14);
	mlx_loop(root.mlx);
	return (0);
}
