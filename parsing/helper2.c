/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:19:57 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/13 16:48:07 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	calculate_y_step(t_ray_data *ray)
{
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_disty = (ray->pos_y - ray->map_y * BLOCK) * ray->delta_disty;
	}
	else
	{
		ray->step_y = 1;
		ray->side_disty = ((ray->map_y + 1) * BLOCK - ray->pos_y)
			* ray->delta_disty;
	}
}

int	check_wall_hit(t_ray_data *ray, t_game *game)
{
	if (ray->map_y < 0 || ray->map_x < 0 || ray->map_y >= game->map_height
		|| ray->map_x >= game->map_width)
	{
		return (1);
	}
	if (game->map[ray->map_y][ray->map_x] == '1')
	{
		return (1);
	}
	return (0);
}

void	perform_dda_step(t_ray_data *ray)
{
	if (ray->side_distx < ray->side_disty)
	{
		ray->side_distx += ray->delta_distx * BLOCK;
		ray->map_x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->side_disty += ray->delta_disty * BLOCK;
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
}

void	run_dda_algorithm(t_ray_data *ray, t_game *game)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		perform_dda_step(ray);
		hit = check_wall_hit(ray, game);
	}
}

void	calculate_perpendicular_distance(t_ray_data *ray)
{
	if (ray->side == 0)
	{
		ray->perp_dist = (ray->map_x * BLOCK - ray->pos_x + (1 - ray->step_x)
				/ 2 * BLOCK) / ray->dir_x;
	}
	else
	{
		ray->perp_dist = (ray->map_y * BLOCK - ray->pos_y + (1 - ray->step_y)
				/ 2 * BLOCK) / ray->dir_y;
	}
}
