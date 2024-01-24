#include "mlx.h"
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MALLOC_ERROR	1
#define	SIDE_LEN		800
#define X				200
#define Y				200

typedef struct s_img
{
	void	*img_ptr;
	char	*img_pixels_ptr;
	int		bits_per_pixel;
	int		endian;
	int		line_len;
}				t_img;

typedef struct	s_var
{
	void	*mlx;
	void	*win;
	t_img	img;
}				t_var;
/*
typedef struct s_game
{
	void	*mlx;
	void	*win;
//	t_player	*p;
//	char**	map;
	void	**asset;
//	int	n_collectables;
}				t_game;
*/

unsigned int	rgb_to_int(int o, int r, int g, int, b)
{
	return (o << 24 | r << 16 | g << 8 | b);
}

//UNDERSTAND
unsigned int	my_pixel_get(t_img *img, int x, int y)
{
	return (*(unsigned int *))
	   	(img->data + (x * img->bpp / 8 + y * img->size_line)));
}

void	my_pixel_put(t_img *img, int x, int y, int color)
{
	int	offset;

	//🚨 Line len is in bytes. WIDTH 800 len_line ~3200 (can differ for alignment)
	offset = (img->line_len * y) + (x * (img->bits_per_pixel / 8));
	//printf("color %d\n", color);
	*((unsigned int *)(offset + img->img_pixels_ptr)) = color;
}

void	texture_load(t_var *data, t_img **img, char *path)
{
	int	width;
	int	height;

	*img = mlx_xpm_file_to_image(data->mlx, path, &width, &height);
//	if (*img = 0)
	//DIDNT USE WIDTH NOR HEIGHT
}

void	draw_duck(t_var *data, t_img *img, int x, init y)
{
	unsigned int	color;
	int	cur_x;
	int	cur_y;

	cur_y = 0;
	while (cur_y < 27)
	{
		cur_x = 0);
		while (cur_x < 24)
		{
			color = mlx_get_pixel(img, cur_x, cur_y);
			if (color != rgb_to_int(0, 255, 255, 255))
				my_pixel_put(&data->img, cur_x, cur_y, color);
			cur_x++;
		}
		cur_y++;
	}
}

int	f(int keysym, t_var *data)
{
	int	what_x;
	int	what_y;	
//	int	step;

	what_x = 200;
	what_y = 200;
	if (keysym == XK_d)
	{
		what_x += step;
		draw_duck(data, &data->img, what_x, what_y);
	}
	/*
	else if (keysym == XK_b)
	{
		color_screen(data, 0xff);
	}*/
	else if (keysym == XK_Escape)
		exit(1);

	// push the READY image to window
	// the last parameters are the offset image-window
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
	return 0;
}


/*
 * All the checks for errors like
 *
 * 	~if (vars.mlx == NULL) exit(1);
 *
 * are not written to not clog te code!
 * But u know this stuff is a necessary evil 😂
*/
int	main()
{
	t_var	vars;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, SIDE_LEN, SIDE_LEN, "My window");
	// Code to create an image and get the related DATA
	vars.img.img_ptr = mlx_new_image(vars.mlx, SIDE_LEN, SIDE_LEN);
	vars.img.img_pixels_ptr = mlx_get_data_addr(vars.img.img_ptr, &vars.img.bits_per_pixel, &vars.img.line_len, &vars.img.endian);
	texture_load(vars, &vars->img, "./assets/idle1.xpm");
	printf("Line_len %d <-> SIDE_LEN %d\n"
			"bpp %d\n"
			"endian %d\n", vars.img.line_len, SIDE_LEN, vars.img.bits_per_pixel, vars.img.endian);
	mlx_key_hook(vars.win, f, &vars);
	mlx_loop(vars.mlx);
}
