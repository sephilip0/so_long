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
//	void	*img_ptr;
	char	*img_pixels_ptr;
	int		bpp;
	int		endian;
	int		line_len;
}				t_img;

typedef struct	s_var
{
	void	*mlx;
	void	*win;
	t_img	*mlx_img;
	t_img	*duck;
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

unsigned int	rgb_to_int(int o, int r, int g, int b)
{
	return (o << 24 | r << 16 | g << 8 | b);
}

//UNDERSTAND
unsigned int	my_pixel_get(t_img *img, int x, int y)
{
	//return (*(unsigned int *)
	//   	(img->img_pixels_ptr + (x * img->bpp / 8 + y * img->line_len)));
	return (*(unsigned int *)
	   	(img->img_pixels_ptr + (x * img->bpp / 8 + y * img->line_len)));
}

void	my_pixel_put(t_img *mlx_img, int x, int y, int color)
{
//	int	offset;

	char		*target;
	//🚨 Line len is in bytes. WIDTH 800 len_line ~3200 (can differ for alignment)
	target = mlx_img->img_pixels_ptr + (x * mlx_img->bpp /  8 + y * mlx_img->line_len);
	*(unsigned int *)target = color;
	//offset = (img->line_len * y) + (x * (img->bpp / 8));
	//*((unsigned int *)(offset + img->img_pixels_ptr)) = color;
}

void	texture_load(t_var *var, t_img **img, char *path)
{
	int	width;
	int	height;

	*img = mlx_xpm_file_to_image(var->mlx, path, &width, &height);
	if (*img == 0)
		printf("BLOW UP\n\n");
	printf("WIDTH: %d\n\n", width);
	printf("HEIGHT: %d\n\n", height);
	//DIDNT USE WIDTH NOR HEIGHT
	printf("bpp: %d\n\n", var->mlx_img->bpp);
	printf("ll: %d\n\n", var->mlx_img->line_len);
}

void	set_texture(t_var *var)
{
	texture_load(var, &var->duck, "./assets/idle1.xpm");
}

void	draw_duck(t_var *var, t_img *img, int x, int y)
{
	unsigned int	color;
	int	cur_x;
	int	cur_y;

	printf("->bpp: %d\n\n", img->bpp);
	printf("->ll: %d\n\n", img->line_len);
	cur_y = 0;
	while (cur_y < 27)
	{
		cur_x = 0;
		while (cur_x < 24)
		{
			color = my_pixel_get(img, cur_x, cur_y);
			if (color != rgb_to_int(0, 255, 255, 255))
				my_pixel_put(var->mlx_img, cur_x + x, cur_y + y, 62440);
			cur_x++;
		}
		cur_y++;
	}
}

void	color_screen(t_var *var, int color)
{
	for (int y = 0; y < SIDE_LEN; ++y)	
	{
		for (int x = 0; x < SIDE_LEN; ++x)
		{
			/*
			 * This function is much faster than the library one🏻
			 * 	~Buffer in the image and push only when ready-> No flickering effect
			*/
			my_pixel_put(var->mlx_img, x, y, color);
		}
	}
}


int	f(int keysym, t_var *var)
{
	int	what_x;
	int	what_y;	
//	int	step;

	what_x = 0;
	what_y = 0;
	if (keysym == XK_d)
	{
		what_x += 200;
		draw_duck(var, var->duck, what_x, what_y);
	}
	/*if (keysym == XK_r)
	{
		color_screen(var, 0xff);
	}*/
	else if (keysym == XK_Escape)
		exit(1);

	// push the READY image to window
	// the last parameters are the offset image-window
	mlx_put_image_to_window(var->mlx, var->win, var->mlx_img, what_x, what_y);
	return 0;
}
/*
void	init_game(t_var *var)
{
	//analyze map


	set_texture(vars);
}*/

void	init_window(t_var *var)
{
	var->mlx = mlx_init();
	if (var->mlx == 0)
		printf("BLOW UP\n\n");
	var->win = mlx_new_window(var->mlx, SIDE_LEN, SIDE_LEN, "so_long");
	if (var->win == 0)
		printf("BLOW UP\n\n");
	var->mlx_img = mlx_new_image(var->mlx, SIDE_LEN, SIDE_LEN);
	if (var->mlx_img == 0)
		printf("BLOW UP\n\n");
}

t_var	*init_var(char *filename)
{
	t_var	*var;

	var = (t_var *)malloc(sizeof(t_var));
	if  (var == 0)
		return (NULL);
		//destroy;
	var->mlx = 0;
	var->win = 0;
	var->mlx_img = 0;
	var->duck = 0;
	init_window(var);
	set_texture(var);
	printf("FILENAME: %s\n\n", filename);
	return (var);
}

int	main()
{
	t_var	*var;

	var = init_var("AV[1]");
//	init_game(&vars);
	//vars.img->img_pixels_ptr = mlx_get_data_addr(vars.img, &vars.img->bpp, &vars.img->line_len, &vars.img->endian);
	mlx_key_hook(var->win, f, var);
	mlx_loop(var->mlx);
	return (0);
	// Code to create an image and get the related DATA
}
