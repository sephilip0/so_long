#include "../includes/so_long.h"

void	set_path(t_image *asset)
{
	asset[0].path = "./assets/diamond_rush/idle1.xpm";
	asset[1].path = "./assets/diamond_rush/idle2.xpm";
	asset[2].path = "./assets/diamond_rush/walk1.xpm";
	asset[3].path = "./assets/diamond_rush/walk2.xpm";
	asset[49].path = "./assets/diamond_rush/wall.xpm";
	asset[50].path = "./assets/diamond_rush/rock.xpm";
	asset[69].path = "./assets/diamond_rush/exit.xpm";
//	asset[1].path = "./assets/duck_idle2.xpm";
	asset[99].path = "./assets/diamond_rush/diamond.xpm";
	asset[111].path = "./assets/diamond_rush/base.xpm";
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

	if (color == 16253176)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_part(t_mlx *root, t_image *image, int x, int y)
{

	//	unsigned int	color;
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
			put_pixel(&root->frame, x + cur_x, y + cur_y, get_pixel_color(image, (cur_x / SCALER), (cur_y / SCALER)));
			cur_x++;
			counter++;
		}
		cur_y++;
	}
	printf("COUNTER: %d\n\n", counter);
	mlx_put_image_to_window(root->mlx, root->window, root->frame.img, 0, 0);
}

void	draw_map(t_mlx *root)
{
// ver qual e o asset
// ver qual e o pixel do asset

	//	unsigned int	color;
	int	cur_x;
	int	cur_y;
	int	i;
	int	j;
	int	character;
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
			character = root->map->map[j][i];
			put_pixel(&root->frame, cur_x, cur_y, get_pixel_color(&root->asset[character], (cur_x % root->s / SCALER), (cur_y % root->s / SCALER)));
			cur_x++;
			counter++;
		}
		cur_y++;
	}
	printf("COUNTER: %d\n\n", counter);
	mlx_put_image_to_window(root->mlx, root->window, root->frame.img, 0, 0);
}
/*
void	framing(t_mlx *root, int width, int height)
{
	root->frame->img = mlx_new_image(root->mlx, 24 * SCALER, 24 * SCALER);
	if (!root->frame->img)
	{
		printf("BOOM2\n");
		//free()
	}
	root->frame->addr = mlx_get_data_addr(root->frame->img, &root->frame->bpp, &root->frame->line_length, &root->frame->endian);
	
	root->frame->height = height * SCALER;
	root->frame->width = width * SCALER;
	printf("frame_width: %d\n", root->frame->width);
	printf("frame_height: %d\n", root->frame->height);
	printf("frame_ll: %d\n", root->frame->line_length);
}*/

void	movement_player(t_mlx *root, int movx, int movy)
{
//	root.frame.player_state_x = movx;
//	root.frame.player_state_y = movy;
	if (root->map->map[root->map->player_y + movy][root->map->player_x + movx] == 'E' && root->map->collect == 0)
	{
		printf("YOU WON! CONGRATS\n");
		exit (0);
	}
	if (root->map->map[root->map->player_y + movy][root->map->player_x + movx] < '1' || 
		root->map->map[root->map->player_y + movy][root->map->player_x + movx] > '9')
	{
		root->map->idle = 0;
		if (root->map->map[root->map->player_y + movy][root->map->player_x + movx] == 'c')
		{
			root->map->map[root->map->player_y + movy][root->map->player_x + movx] = 'o';
			root->map->collect -= 1;
		}
		printf("nextx: %d\n", (root->map->player_x * root->s + (movx * 24)));
		printf("nexty: %d\n", (root->map->player_y * root->s + (movy * 24)));
		draw_part(root, &root->asset[111], root->map->player_x * root->s, root->map->player_y * root->s);
		//draw_part(root, &root->asset[2], root->map->player_x * root->s + (movx * 24), root->map->player_y * root->s + (movy * 24));
		draw_part(root, &root->asset[3], root->map->player_x * root->s + (movx * 24), root->map->player_y * root->s + (movy * 24));
		root->map->player_y += movy;
		root->map->player_x += movx;
		//ATENTION HERE
	//	root->map->player_state_x = 0;
	//	root->map->player_state_y = 0;
	}
//	if (root->map->map[root->map->exit_y][root->map->exit_x] != 'P')
//		root->map->map[root->map->exit_y][root->map->exit_x] = 'E';
	printf("player_x: %d\n", root->map->player_x);
	printf("player_y: %d\n", root->map->player_y);
	printf("COLLECTABLES: %d\n", root->map->collect);
	ft_putmat(root->map->map);
}


int	input_player(int keysym, t_mlx *root)
{
//	clock_t	time;

	if (keysym == XK_w)
	{
		printf("go up \n");
	//	root->map->player_state_x = 0;
	//	root->map->player_state_y = -1;
		movement_player(root, 0, -1);
	}
	else if (keysym == XK_s)
	{
		printf("go down \n");
	//	root->map->player_state_x = 0;
	//	root->map->player_state_y = 1;
		movement_player(root, 0, 1);
	}
	else if (keysym == XK_a)
	{
		printf("go left \n");
	//	root->map->player_state_x = -1;
	//	root->map->player_state_y = 0;
		movement_player(root, -1, 0);
	}
	else if (keysym == XK_d)
	{
		printf("go right \n");
	//	root->map->player_state_x = 1;
	//	root->map->player_state_y = 0;
		movement_player(root, 1, 0);
	}
	else if (keysym == XK_Escape)
		exit(1);
	//NOT EXIT, RETURN 1?
	// else change state
//	draw_part(root, &root->asset[1], root->map->player_x * root->s, root->map->player_y * root->s);
	//FOR THE SAKE OF UPDATE
//	checkgravity(root);
//	mlx_put_image_to_window(root->mlx, root->window, root->frame.img, 0, 0);
	return (0);
}
/*
void	render(t_mlx *root)
{
}*/

int	looper(t_mlx *root)
{
	struct timespec instant;

	clock_gettime(CLOCK_REALTIME, &instant);
	//printf("IS IT CLOCK_GETTIME?\n");
	if ((root->dif_timer != instant.tv_nsec / 100000000) && (instant.tv_nsec / 100000000 % 1 == 0))
	{
		if (root->map->idle == 0)
		{
			draw_map(root);
			draw_part(root, &root->asset[2], root->map->player_x * root->s, root->map->player_y * root->s);
			root->map->idle = 1;
		}
		//ONE FUNCTION FOR ALL IDLE INCLUDING GRAVITY?
		else if (instant.tv_nsec / 100000000 % 5 == 0 && root->map->idle == 1) 
		{
			draw_part(root, &root->asset[111], root->map->player_x * root->s, root->map->player_y * root->s);
			draw_part(root, &root->asset[0], root->map->player_x * root->s, root->map->player_y * root->s);
			root->map->idle = 2;
		}
		else if (instant.tv_nsec / 100000000 % 5 == 0 && root->map->idle == 2)
		{
			draw_part(root, &root->asset[111], root->map->player_x * root->s, root->map->player_y * root->s);
			draw_part(root, &root->asset[1], root->map->player_x * root->s, root->map->player_y * root->s);
			root->map->idle = 1;
		}
		root->dif_timer = instant.tv_nsec / 100000000; 
	}
	return (0); 
}

//NOT IN .H
void printtmap(t_map *map)
{
	printf("collect: %d\n", map->collect);
	printf("height: %d\n", map->height);
	printf("width: %d\n", map->width);
	printf("player_x: %d\n", map->player_x);
	printf("player_y: %d\n", map->player_y);
	printf("idle: %d\n", map->idle);
	printf("active_exit: %d\n", map->active_exit);
	printf("exit_x: %d\n", map->exit_x);
	printf("exit_y: %d\n", map->exit_y);
	ft_putmat(map->map);
}

int main(int argc, char *argv[])
{
	t_mlx	root;
	t_image	frame;
	t_map	map;

	if (argc != 2)
	{
		printf("NOT THE RIGHT ARGUMENTS");
		return (0);	
	}
	root.mlx = mlx_init();
	// 1000 will be x * 24 * Scaler?
	root.window = mlx_new_window(root.mlx, 1000, 1000, "duckgame");
	root.asset = init_assets(&root);
	root.x = 0;
	root.y = 0;
	root.s = 24 * SCALER;
	root.dif_timer = 0;
	//AFTER SETITNG MAP->PLAYER_X AND Y REMOVE P FROM MAP
	//printf("WHAT\n\n")1111111111111
	map_constructor(&map, argv[1]);
	root.map = &map;
	printtmap(&map);
	frame.height = root.map->height * root.s;
	frame.width = root.map->width * root.s;
	frame.img = mlx_new_image(root.mlx, frame.width, frame.height);
	frame.addr = mlx_get_data_addr(frame.img, &frame.bpp, &frame.line_length, &frame.endian);
	//mlx_put_image_to_window(root.mlx, root.window, root.asset[1].img, 500 - 12, 500 - 14);
	root.frame = frame;
	printf("root->s: %d\n", root.s);
	printf("frame %d e %d\n", root.frame.width, root.frame.height);
	printf("asset[1]: %d e %d\n", root.asset[111].width, root.asset[111].height);
	mlx_key_hook(root.window, input_player, &root);
	mlx_loop_hook(root.mlx, looper, &root);	
	mlx_loop(root.mlx);
	//FREE STUFF MAP AND ASSETS
	free_map(map.map);
	return (0);
}
