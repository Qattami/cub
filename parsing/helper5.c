/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:48:13 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/13 16:50:26 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	apply_side_shading(int color, int side)
{
	if (side == 1)
	{
		color = ((color >> 1) & 0x7F7F7F);
	}
	return (color);
}

int	apply_distance_fade(int color, float distance)
{
	float	fade;
	int		r_comp;
	int		g_comp;
	int		b_comp;

	if (distance < 5.0f)
	{
		fade = distance / 5.0f;
		r_comp = ((color >> 16) & 0xFF) * fade;
		g_comp = ((color >> 8) & 0xFF) * fade;
		b_comp = (color & 0xFF) * fade;
		color = (r_comp << 16) | (g_comp << 8) | b_comp;
	}
	return (color);
}

int	calculate_color_with_shading(t_texture_data *tex, int tex_index, int side,
		float distance)
{
	unsigned char	b;
	unsigned char	g;
	unsigned char	r;
	int				color;

	b = tex->data[tex_index];
	g = tex->data[tex_index + 1];
	r = tex->data[tex_index + 2];
	color = (r << 16) | (g << 8) | b;
	color = apply_side_shading(color, side);
	color = apply_distance_fade(color, distance);
	return (color);
}

void	setup_texture_step(t_wall_data *wall, t_texture_data *tex, double *step,
		double *texPos)
{
	*step = (double)tex->height / wall->line_height;
	*texPos = (wall->draw_start - HEIGHT / 2.0 + wall->line_height / 2.0)
		* (*step);
	if (*texPos < 0)
	{
		*texPos = 0;
	}
}

void	draw_wall_pixel(t_render_context *ctx, int column, int y, double texPos)
{
	int	tex_y;
	int	tex_index;
	int	color;

	tex_y = (int)texPos;
	if (tex_y >= ctx->tex->height)
	{
		tex_y = ctx->tex->height - 1;
	}
	if (tex_y < 0)
	{
		tex_y = 0;
	}
	tex_index = tex_y * ctx->tex->size_line + ctx->wall->tex_x * (ctx->tex->bpp
			/ 8);
	if (tex_index >= 0 && tex_index < (ctx->tex->size_line * ctx->tex->height
			- 4))
	{
		color = calculate_color_with_shading(ctx->tex, tex_index,
				ctx->ray->side, ctx->ray->distance);
		put_pixel(column, y, color, ctx->game);
	}
	else
		put_pixel(column, y, 0x808080, ctx->game);
}
