#include "../includes/so_long.h"

void	set_path(t_image *asset)
{
	asset[1].path = "./assets/diamond_rush/idle1.xpm";
	asset[2].path = "./assets/diamond_rush/idle2.xpm";
	asset[3].path = "./assets/diamond_rush/walkr1.xpm";
	asset[4].path = "./assets/diamond_rush/walkr2.xpm";
	asset[5].path = "./assets/diamond_rush/walkl1.xpm";
	asset[6].path = "./assets/diamond_rush/walkl2.xpm";
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
		exitmap(root->map->map, 1, "NO ASSETS\n");
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

//lr means left right
void	movement_player(t_mlx *root, int movx, int movy, int lr)
{
	if (root->map->map[root->map->player_y + movy][root->map->player_x + movx] == 'E' && \
	root->map->collect == 0)
	{
		write(1, "YOU WON! CONGRATS\n", 18);
		killmlx(root, 2, 0);
	}
	if (root->map->map[root->map->player_y + movy][root->map->player_x + movx] < '1' || 
		root->map->map[root->map->player_y + movy][root->map->player_x + movx] > '9')
	{
		root->map->idle = 0;
		root->steps += 1;
		if (root->map->map[root->map->player_y + movy][root->map->player_x + movx] == 'c')
		{
			root->map->map[root->map->player_y + movy][root->map->player_x + movx] = 'o';
			root->map->collect -= 1;
		}
		draw_part(root, &root->asset[111], root->map->player_x * root->s, root->map->player_y * root->s);
		draw_part(root, &root->asset[lr], root->map->player_x * root->s + (movx * 24), root->map->player_y * root->s + (movy * 24));
		root->map->walk = lr - 1;
		root->map->player_y += movy;
		root->map->player_x += movx;
		if (!STEPS_ON_SCREEN)
			ft_printf("Number of Moves: %d\n", root->steps);
	}
}

int	input_player(int keysym, t_mlx *root)
{
	if (keysym == XK_w)
		movement_player(root, 0, -1, 4);
	else if (keysym == XK_s)
		movement_player(root, 0, 1, 4);
	else if (keysym == XK_a)
		movement_player(root, -1, 0, 6);
	else if (keysym == XK_d)
		movement_player(root, 1, 0, 4);
	else if (keysym == XK_Escape)
		killmlx(root, 2, 0);
	return (0);
}

int	switcher(int base, int min, int max)
{
	if (min > max || base < min || base > max)
		return (base);
	if (base == max)
		base = min;
	else
		base++;
	return (base);
}

int	looper(t_mlx *root)
{
	struct timespec	instant;

	clock_gettime(CLOCK_REALTIME, &instant);
	print_steps(root);
	if ((root->dif_timer != instant.tv_nsec / 100000000) && \
	(instant.tv_nsec / 100000000 % 1 == 0))
	{
		if (root->map->idle == 0)
		{
			draw_map(root, 0);
			draw_part(root, &root->asset[root->map->walk], \
			root->map->player_x * root->s, root->map->player_y * root->s);
			root->map->idle = 1;
		}
		else if (instant.tv_nsec / 100000000 % 5 == 0) 
		{
			draw_part(root, &root->asset[111], root->map->player_x * root->s, \
			root->map->player_y * root->s);
			draw_part(root, &root->asset[root->map->idle], \
			root->map->player_x * root->s, root->map->player_y * root->s);
			root->map->idle = switcher(root->map->idle, 1, 2);
		}
		root->dif_timer = instant.tv_nsec / 100000000; 
	}
	return (0); 
}

void	root_constructor(t_mlx *root, t_map *map, t_image *frame)
{
	root->map = map;
	root->mlx = mlx_init();
	if (!root->mlx)
		exitmap(root->map->map, 1, "NO ROOT->MLX\n");
	root->asset = init_assets(root);
	root->s = 24 * SCALER;
	frame->height = root->map->height * root->s;
	frame->width = root->map->width * root->s;
	frame->img = mlx_new_image(root->mlx, frame->width, frame->height);
	if (!frame->img)
		killmlx(root, 0, 1);
	frame->addr = mlx_get_data_addr(frame->img, &frame->bpp, \
	&frame->line_length, &frame->endian);
	root->frame = *frame;
	root->window = mlx_new_window(root->mlx, frame->width, \
	frame->height, "so_long");
	if (!root->window)
		killmlx(root, 1, 1);
	root->steps = 0;
	root->dif_timer = 0;
}

void	print_steps(t_mlx *root)
{
	char	*moves;

	if(STEPS_ON_SCREEN)
	{
		moves = ft_itoa(root->steps);
		mlx_string_put(root->mlx, root->window, 10, 10,
			0x000000, "Number of Moves:");
		mlx_string_put(root->mlx, root->window, 110, 10,
			0x000000, moves);
		free(moves);
	}
}

int	main(int argc, char *argv[])
{
	t_mlx	root;
	t_image	frame;
	t_map	map;

	if (argc != 2)
	{
		write(1, "NOT THE RIGHT ARGUMENTS", 23);
		return (0);
	}
	map_constructor(&map, argv[1]);
	root_constructor(&root, &map, &frame);
	mlx_hook(root.window, 17, 1L << 0, exit_game, &root);
	mlx_key_hook(root.window, input_player, &root);
	mlx_loop_hook(root.mlx, looper, &root);
	mlx_loop(root.mlx);
	return (0);
}