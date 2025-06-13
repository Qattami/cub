/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:19:59 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/13 16:50:01 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	apply_fisheye_correction(t_ray_data *ray, t_player *player,
		float ray_angle)
{
	ray->distance = fabs(ray->perp_dist * cos(ray_angle - player->angle));
	if (ray->distance < 0.1f)
	{
		ray->distance = 0.1f;
	}
}

void	draw_debug_ray_step(t_ray_data *ray, t_game *game, float *ray_x,
		float *ray_y)
{
	put_pixel(*ray_x, *ray_y, 0xFF0000, game);
	*ray_x += ray->dir_x * 0.5f;
	*ray_y += ray->dir_y * 0.5f;
}

void	draw_debug_line(t_ray_data *ray, t_game *game)
{
	float	ray_x;
	float	ray_y;
	int		steps;

	ray_x = ray->pos_x;
	ray_y = ray->pos_y;
	steps = 0;
	while (!touch(ray_x, ray_y, game) && steps < 1000)
	{
		draw_debug_ray_step(ray, game, &ray_x, &ray_y);
		steps++;
	}
}

void	calculate_line_height(t_wall_data *wall, float distance)
{
	wall->line_height = (BLOCK * HEIGHT) / (distance + 0.01f);
	if (wall->line_height > HEIGHT * 2)
	{
		wall->line_height = HEIGHT * 2;
	}
}

void	calculate_draw_bounds(t_wall_data *wall)
{
	wall->draw_start = (HEIGHT - (int)wall->line_height) / 2;
	wall->draw_end = wall->draw_start + (int)wall->line_height;
	if (wall->draw_start < 0)
	{
		wall->draw_start = 0;
	}
	if (wall->draw_end >= HEIGHT)
	{
		wall->draw_end = HEIGHT - 1;
	}
}
