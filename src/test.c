#include "../includes/so_long.h"

//COPY OF MAIN.C
static void	ft_putmat(char **mat)
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
		write(1, "\n", 1);
		i++;
	}
}

void	set_path(t_image *asset)
{
	asset[49].path = "./assets/diamond_rush/wall.xpm";
	asset[50].path = "./assets/diamond_rush/rock.xpm";
	asset[80].path = "./assets/diamond_rush/player1.xpm";
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

//	if (color == 0)
//		return ;
	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_xy(t_mlx *root, t_image *asset, int x, int y)
{
//	unsigned int	color;
	int	counter;
	int	cur_x;
	int	cur_y;

	counter = 0;
	cur_y = 0;
	while (cur_y < root->frame.height)
	{
		cur_x = 0;
		while (cur_x < root->frame.width)
		{
		//	color = get_pixel_color(asset, cur_x, cur_y);
		//if (color != rgb_to_int(0, 255, 255, 255))
			put_pixel(&root->frame, cur_x, cur_y, get_pixel_color(asset, cur_x / SCALER, cur_y / SCALER));
			cur_x++;
			counter++;
		}
		cur_y++;
	}
	printf("\n%d,%d\n", x, y);
	printf("counter: %d\n", counter);
	mlx_put_image_to_window(root->mlx, root->window, asset->img, x, y);
}

void	draw(t_mlx *root)
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
		//	printf("i: %d j: %d\n\n", i, j);
		//	printf("character: %d\n\n", character);
//			printf("(char: %i)", character);
		//	color = get_pixel_color(asset, cur_x, cur_y);
			//if (color != rgb_to_int(0, 255, 255, 255))
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
	root.frame.player_state_x = movx;
	root.frame.player_state_y = movy;
	if (root->map->map[root->map->player_y + movy][root->map->player_x + movx] == 'E' && root->map->collect == 0)
	{
		printf("YOU WON! CONGRATS\n");
		exit (0);
	}
	if (root->map->map[root->map->player_y + movy][root->map->player_x + movx] <= '1' || 
		root->map->map[root->map->player_y + movy][root->map->player_x + movx] >= '9')
	{
		if (root->map->map[root->map->player_y + movy][root->map->player_x + movx] == 'c')
			root->map->collect -= 1;
		root->map->map[root->map->player_y][root->map->player_x] = 'o';
		root->map->player_y += movy;
		root->map->player_x += movx;
		root->map->map[root->map->player_y][root->map->player_x] = 'P';
	}
	if (root->map->map[root->map->exit_y][root->map->exit_x] != 'P')
		root->map->map[root->map->exit_y][root->map->exit_x] = 'E';
	printf("player_x: %d\n", root->map->player_x);
	printf("player_y: %d\n", root->map->player_y);
	printf("COLLECTABLES: %d\n", root->map->collect);
	ft_putmat(root->map->map);
}


int	input_player(int keysym, t_mlx *root)
{
	if (keysym == XK_w)
	{
		printf("go up \n");
		movement_player(root, 0, -1);
	}
	else if (keysym == XK_s)
	{
		printf("go down \n");
		movement_player(root, 0, 1);
	}
	else if (keysym == XK_a)
	{
		printf("go left \n");
		movement_player(root, -1, 0);
	}
	else if (keysym == XK_d)
	{
		printf("go right \n");
		movement_player(root, +1, 0);
	}
	else if (keysym == XK_Escape)
		exit(1);
	else
		movement_player(root, 0, 0);
	//FOR THE SAKE OF UPDATE
//	checkgravity(root);
	draw(root);
//	mlx_put_image_to_window(root->mlx, root->window, root->frame.img, 0, 0);
	return (0);
}


int main()
{
	t_mlx	root;
	t_image	frame;

	root.mlx = mlx_init();
	// 1000 will be x * 24 * Scaler?
	root.window = mlx_new_window(root.mlx, 1000, 1000, "duckgame");
//	duck.path = "./assets/idle1.xpm";
//	duck.img = mlx_new_image(root.mlx, 24, 27);
//	duck.img = mlx_xpm_file_to_image(root.mlx, duck.path, &duck.width, &duck.height);
//	duck.addr = mlx_get_data_addr(duck.img, &duck.bpp, &duck.line_length, &duck.endian);
	root.asset = init_assets(&root);
	root.x = 0;
	root.y = 0;
	root.s = 24 * SCALER;
	root.map->map = ft_split("11111 1Poc1 1o221 1co21 oEooc 11111", ' ');
	root.map->player_x = 1;
	root.map->player_y = 1;
	root.map->collect = 3;
	root.map->exit_x = 1;
	root.map->exit_y = 4;
	ft_putmat(root.map->map);
	printf("\n");
	frame.height = 6 * root.s;
	frame.width = 5 * root.s;
	frame.img = mlx_new_image(root.mlx, frame.width, frame.height);
	frame.addr = mlx_get_data_addr(frame.img, &frame.bpp, &frame.line_length, &frame.endian);
	//mlx_put_image_to_window(root.mlx, root.window, root.asset[1].img, 500 - 12, 500 - 14);
	root.frame = frame;
	printf("root->s: %d\n", root.s);
	printf("frame %d e %d\n", root.frame.width, root.frame.height);
	printf("asset[1]: %d e %d\n", root.asset[111].width, root.asset[111].height);
//	draw_xy(&root, &root.asset[111], 500, 500);
	draw(&root);
	sleep(1);
//	draw_xy(&root, &root.asset[1], root.x, root.y);
//	draw_xy(&root, &root.asset[0], 324, 300);
//	draw_xy(&root, &root.asset[0], 348, 300);
//	sleep(0.1);
//	draw_xy(&root, &root.asset[2], 500, 500);
//	draw(&root, &asset[1]);
//	mlx_put_image_to_window(root.mlx, root.window, asset[0].img, 500 - 12, 500 - 14);
//	sleep(1);
//	mlx_put_image_to_window(root.mlx, root.window, asset[1].img, 200 - 12, 200 - 14);
	//
	mlx_key_hook(root.window, input_player, &root);
	mlx_loop(root.mlx);
	return (0);
}
