/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:19:52 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/13 16:52:39 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	draw_map(t_game *game)
{
	char	**map;
	int		color;
	int		y;
	int		x;

	map = game->map;
	color = 0x0000FF;
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			if (map[y][x] == '1')
				draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
			x++;
		}
		y++;
	}
}

float	distance(float x, float y)
{
	return (sqrt(x * x + y * y));
}

bool	check_adjacent(t_game *game, int grid_x, int grid_y, float sub_x,
		float sub_y)
{
	if (sub_x < 0.05 && grid_x > 0 && game->map[grid_y][grid_x - 1] == '1')
		return (true);
	if (sub_x > 0.95 && grid_x < game->map_width - 1 && game->map[grid_y][grid_x
		+ 1] == '1')
		return (true);
	if (sub_y < 0.05 && grid_y > 0 && game->map[grid_y - 1][grid_x] == '1')
		return (true);
	if (sub_y > 0.95 && grid_y < game->map_height - 1 && game->map[grid_y
			+ 1][grid_x] == '1')
		return (true);
	return (false);
}

bool	touch(float px, float py, t_game *game)
{
	int		grid_x;
	int		grid_y;
	float	sub_x;
	float	sub_y;

	grid_x = (int)(px / BLOCK);
	grid_y = (int)(py / BLOCK);
	if (grid_y < 0 || grid_y >= game->map_height || grid_x < 0
		|| grid_x >= game->map_width)
		return (true);
	if (game->map[grid_y][grid_x] == '1')
		return (true);
	sub_x = fmod(px, BLOCK) / BLOCK;
	sub_y = fmod(py, BLOCK) / BLOCK;
	return (check_adjacent(game, grid_x, grid_y, sub_x, sub_y));
}

void	init_game(t_game *game, t_map *map, t_parse *color)
{
	init_player(&game->player, map->player_dir, map);
	game->map = map->map;
	game->map_width = map->map_w;
	game->map_height = map->map_h;
	game->floor_color = get_color(color, 'F');
	game->ceiling_color = get_color(color, 'C');
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			&game->endian);
	load_all_textures(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
