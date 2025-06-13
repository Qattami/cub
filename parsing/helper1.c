/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:50:40 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/13 16:46:16 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	draw_loop(t_game *game)
{
	t_player	*player;
	float		ray_start;
	float		ray_step;
	int			x;
	float		fov;

	player = &game->player;
	move_player(player, game);
	clear_image(game);
	fov = PI / 3;
	ray_start = player->angle - fov / 2;
	ray_step = fov / WIDTH;
	x = 0;
	while (x < WIDTH)
	{
		draw_line(player, game, ray_start, x);
		ray_start += ray_step;
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

void	init_ray_direction(t_ray_data *ray, float ray_angle)
{
	ray->dir_x = cos(ray_angle);
	ray->dir_y = sin(ray_angle);
}

void	init_ray_position(t_ray_data *ray, t_player *player)
{
	ray->pos_x = player->x;
	ray->pos_y = player->y;
	ray->map_x = (int)(ray->pos_x / BLOCK);
	ray->map_y = (int)(ray->pos_y / BLOCK);
}

void	calculate_delta_distances(t_ray_data *ray)
{
	if (ray->dir_x == 0)
	{
		ray->delta_distx = 1e30;
	}
	else
	{
		ray->delta_distx = fabs(1 / ray->dir_x);
	}
	if (ray->dir_y == 0)
	{
		ray->delta_disty = 1e30;
	}
	else
	{
		ray->delta_disty = fabs(1 / ray->dir_y);
	}
}

void	calculate_x_step(t_ray_data *ray)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_distx = (ray->pos_x - ray->map_x * BLOCK) * ray->delta_distx;
	}
	else
	{
		ray->step_x = 1;
		ray->side_distx = ((ray->map_x + 1) * BLOCK - ray->pos_x)
			* ray->delta_distx;
	}
}
