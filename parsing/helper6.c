/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:48:45 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/13 16:50:49 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	draw_wall_column(t_render_context *ctx, int column)
{
	double	step;
	double	tex_pos;
	int		y;

	setup_texture_step(ctx->wall, ctx->tex, &step, &tex_pos);
	y = ctx->wall->draw_start;
	while (y < ctx->wall->draw_end && y < HEIGHT)
	{
		draw_wall_pixel(ctx, column, y, tex_pos);
		tex_pos += step;
		y++;
	}
}

void	draw_ceiling(t_wall_data *wall, int column, t_game *game)
{
	int	y;

	y = 0;
	while (y < wall->draw_start)
	{
		put_pixel(column, y, game->ceiling_color, game);
		y++;
	}
}

void	draw_floor(t_wall_data *wall, int column, t_game *game)
{
	int	y;

	y = wall->draw_end;
	while (y < HEIGHT)
	{
		put_pixel(column, y, game->floor_color, game);
		y++;
	}
}

void	setup_ray_data(t_ray_data *ray, t_player *player, float ray_angle)
{
	init_ray_direction(ray, ray_angle);
	init_ray_position(ray, player);
	calculate_delta_distances(ray);
	calculate_x_step(ray);
	calculate_y_step(ray);
}

void	perform_wall_calculations(t_wall_data *wall, t_ray_data *ray,
		t_game *game)
{
	calculate_line_height(wall, ray->distance);
	calculate_draw_bounds(wall);
	calculate_wall_x_coordinate(wall, ray);
	select_wall_texture(wall, ray, game);
}
