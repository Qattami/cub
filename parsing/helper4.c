/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:20:02 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/13 16:49:51 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	calculate_wall_x_coordinate(t_wall_data *wall, t_ray_data *ray)
{
	if (ray->side == 0)
	{
		wall->wall_x = ray->pos_y + ray->perp_dist * ray->dir_y;
	}
	else
	{
		wall->wall_x = ray->pos_x + ray->perp_dist * ray->dir_x;
	}
	wall->wall_x = wall->wall_x - floor(wall->wall_x / BLOCK) * BLOCK;
	wall->wall_x = wall->wall_x / BLOCK;
	if (wall->wall_x < 0.0f)
	{
		wall->wall_x = 0.0f;
	}
	if (wall->wall_x > 1.0f)
	{
		wall->wall_x = 1.0f;
	}
}

void	select_wall_texture(t_wall_data *wall, t_ray_data *ray, t_game *game)
{
	if (ray->side == 1)
	{
		if (ray->dir_y > 0)
		{
			wall->texture_img = game->textures.so_img;
		}
		else
		{
			wall->texture_img = game->textures.no_img;
		}
	}
	else
	{
		if (ray->dir_x > 0)
		{
			wall->texture_img = game->textures.ea_img;
		}
		else
		{
			wall->texture_img = game->textures.we_img;
		}
	}
}

void	calculate_texture_x_coordinate(t_wall_data *wall, int tex_width)
{
	wall->tex_x = (int)(wall->wall_x * tex_width);
	if (wall->tex_x >= tex_width)
	{
		wall->tex_x = tex_width - 1;
	}
	if (wall->tex_x < 0)
	{
		wall->tex_x = 0;
	}
}

void	apply_texture_flipping(t_wall_data *wall, t_ray_data *ray,
		int tex_width)
{
	if (ray->side == 0 && ray->dir_x > 0)
	{
		wall->tex_x = tex_width - wall->tex_x - 1;
	}
	if (ray->side == 1 && ray->dir_y < 0)
	{
		wall->tex_x = tex_width - wall->tex_x - 1;
	}
}

int	get_texture_data(t_texture_data *tex, void *img, t_game *game)
{
	int	endian;

	tex->data = mlx_get_data_addr(img, &tex->bpp, &tex->size_line, &endian);
	if (!tex->data)
	{
		return (0);
	}
	tex->width = game->textures.tex_width;
	tex->height = game->textures.tex_height;
	return (1);
}
