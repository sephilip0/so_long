/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behindscreen.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sephilip <sephlip@student.42lisboa.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:13:57 by sephilip          #+#    #+#             */
/*   Updated: 2024/04/11 12:13:58 by sephilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

//lr means left right
void	movement_player(t_mlx *r, int movx, int movy, int lr)
{
	if (r->map->map[r->map->p_y + movy][r->map->p_x + movx] == 'E' && \
	r->map->collect == 0)
	{
		write(1, "YOU WON! CONGRATS\n", 18);
		killmlx(r, 2, 0);
	}
	if (r->map->map[r->map->p_y + movy][r->map->p_x + movx] < '1' || 
		r->map->map[r->map->p_y + movy][r->map->p_x + movx] > '9')
	{
		r->map->idle = 0;
		r->steps += 1;
		if (r->map->map[r->map->p_y + movy][r->map->p_x + movx] == 'c')
		{
			r->map->map[r->map->p_y + movy][r->map->p_x + movx] = 'o';
			r->map->collect -= 1;
		}
		draw_part(r, &r->asset[111], r->map->p_x * r->s, r->map->p_y * r->s);
		draw_part(r, &r->asset[lr], r->map->p_x * r->s + (movx * 24), \
		r->map->p_y * r->s + (movy * 24));
		r->map->walk = lr - 1;
		r->map->p_y += movy;
		r->map->p_x += movx;
		if (!STEPS_ON_SCREEN)
			ft_printf("Number of Moves: %d\n", r->steps);
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
			root->map->p_x * root->s, root->map->p_y * root->s);
			root->map->idle = 1;
		}
		else if (instant.tv_nsec / 100000000 % 5 == 0) 
		{
			draw_part(root, &root->asset[111], root->map->p_x * root->s, \
			root->map->p_y * root->s);
			draw_part(root, &root->asset[root->map->idle], \
			root->map->p_x * root->s, root->map->p_y * root->s);
			root->map->idle = switcher(root->map->idle, 1, 2);
		}
		root->dif_timer = instant.tv_nsec / 100000000; 
	}
	return (0); 
}

void	print_steps(t_mlx *root)
{
	char	*moves;

	if (STEPS_ON_SCREEN)
	{
		moves = ft_itoa(root->steps);
		mlx_string_put(root->mlx, root->window, 10, 10,
			0x000000, "Number of Moves:");
		mlx_string_put(root->mlx, root->window, 110, 10,
			0x000000, moves);
		free(moves);
	}
}
