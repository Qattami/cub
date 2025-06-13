/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozouine <ozouine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:22:01 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/11 22:55:47 by ozouine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	draw_line(t_player *player, t_game *game, float ray_angle, int column)
{
	t_ray_data			ray;
	t_wall_data			wall;
	t_texture_data		tex;
	t_render_context	ctx;

	setup_ray_data(&ray, player, ray_angle);
	run_dda_algorithm(&ray, game);
	calculate_perpendicular_distance(&ray);
	apply_fisheye_correction(&ray, player, ray_angle);
	perform_wall_calculations(&wall, &ray, game);
	if (!get_texture_data(&tex, wall.texture_img, game))
	{
		printf("Error: Could not get texture data\n");
		return ;
	}
	calculate_texture_x_coordinate(&wall, tex.width);
	apply_texture_flipping(&wall, &ray, tex.width);
	ctx.ray = &ray;
	ctx.wall = &wall;
	ctx.tex = &tex;
	ctx.game = game;
	draw_wall_column(&ctx, column);
	draw_ceiling(&wall, column, game);
	draw_floor(&wall, column, game);
}
