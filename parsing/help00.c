/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help00.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:04:40 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/13 01:48:27 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	init_var(t_map *data)
{
	data->count = 0;
	data->i = 0;
	data->j = 0;
}

int	end_of_read_lines(char *line, char **t)
{
	free(line);
	t[0] = NULL;
	return (1);
}

char	*helper31(char *line, int k)
{
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	return (ft_strdup(&line[k]));
}

void	free_map_data(t_map *data)
{
	if (!data)
		return ;
	if (data->map)
		free_tab(data->map);
	free(data);
}

// void	free_textures(t_textures *tex, t_game *game)
// {
// 	if (tex->no_path)
// 		free(tex->no_path);
// 	if (tex->so_path)
// 		free(tex->so_path);
// 	if (tex->we_path)
// 		free(tex->we_path);
// 	if (tex->ea_path)
// 		free(tex->ea_path);
// 	if (game && game->mlx)
// 	{
// 		if (tex->no_img)
// 			mlx_destroy_image(game->mlx, tex->no_img);
// 		if (tex->so_img)
// 			mlx_destroy_image(game->mlx, tex->so_img);
// 		if (tex->we_img)
// 			mlx_destroy_image(game->mlx, tex->we_img);
// 		if (tex->ea_img)
// 			mlx_destroy_image(game->mlx, tex->ea_img);
// 	}
// 	tex->no_path = NULL;
// 	tex->so_path = NULL;
// 	tex->we_path = NULL;
// 	tex->ea_path = NULL;
// 	tex->no_img = NULL;
// 	tex->so_img = NULL;
// 	tex->we_img = NULL;
// 	tex->ea_img = NULL;
// }

// void	cleanup_game(t_game *game, t_map *map_data, t_parse *parse_data)
// {
// 	if (game)
// 	{
// 		free_textures(&game->textures, game);
// 		if (game->img && game->mlx)
// 			mlx_destroy_image(game->mlx, game->img);
// 		if (game->win && game->mlx)
// 			mlx_destroy_window(game->mlx, game->win);
// 		if (game->mlx)
// 			mlx_destroy_display(game->mlx);
// 	}
// 	if (map_data)
// 		free_map_data(map_data);

// 	if (parse_data)
// 	{
// 		if (parse_data->next)
// 			free(parse_data->next);
// 		free(parse_data);
// 	}
// }