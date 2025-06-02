#include "parsing.h"

void	load_texture(t_game *game, void **img, char *path)
{
	int width, height;
	*img = mlx_xpm_file_to_image(game->mlx, path, &width, &height);
	if (*img == NULL)
	{
		printf("Error: failed to load texture: %s\n", path);
		exit(1);
	}
	// Only set dimensions once (from first texture)
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
	// Initialize dimensions to 0
	game->textures.tex_width = 0;
	game->textures.tex_height = 0;

	load_texture(game, &game->textures.no_img, game->textures.no_path);
	load_texture(game, &game->textures.so_img, game->textures.so_path);
	load_texture(game, &game->textures.we_img, game->textures.we_path);
	load_texture(game, &game->textures.ea_img, game->textures.ea_path);

	printf("All textures loaded successfully!\n");
}