/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 02:58:58 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/03 03:00:19 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	load_texture(t_game *game, void **img, char *path)
{
	int	width;
	int	height;

	*img = mlx_xpm_file_to_image(game->mlx, path, &width, &height);
	if (*img == NULL)
	{
		printf("Error: failed to load texture: %s\n", path);
		exit(1);
	}
	if (game->textures.tex_width == 0)
	{
		game->textures.tex_width = width;
		game->textures.tex_height = height;
		printf("Texture dimensions set to: %dx%d\n", width, height);
	}
	printf("Loaded texture %s: %dx%d\n", path, width, height);
}

void	load_all_textures(t_game *game)
{
	game->textures.tex_width = 0;
	game->textures.tex_height = 0;
	load_texture(game, &game->textures.no_img, game->textures.no_path);
	load_texture(game, &game->textures.so_img, game->textures.so_path);
	load_texture(game, &game->textures.we_img, game->textures.we_path);
	load_texture(game, &game->textures.ea_img, game->textures.ea_path);
	printf("All textures loaded successfully!\n");
}
