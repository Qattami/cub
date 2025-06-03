#include "parsing.h"

// Convert RGB to hexadecimal color
int	rgb_to_hex(int r, int g, int b)
{
	if (r < 0)
		r = 0;
	else if (r > 255)
		r = 255;
	if (g < 0)
		g = 0;
	else if (g > 255)
		g = 255;
	if (b < 0)
		b = 0;
	else if (b > 255)
		b = 255;
	return ((r << 16) | (g << 8) | b);
}

// Get color from parse list
int	get_color(t_parse *color_list, char type)
{
	t_parse	*current;

	current = color_list;
	while (current != NULL)
	{
		if (current->f_c == type)
		{
			return (rgb_to_hex(current->r, current->g, current->b));
		}
		current = current->next;
	}
	// Default colors if not found
	if (type == 'F')
	{
		return (rgb_to_hex(139, 69, 19)); // Brown floor
	}
	else
	{
		return (rgb_to_hex(135, 206, 235)); // Blue ceiling
	}
}

// our own put_pixel
void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	index = y * game->size_line + x * game->bpp / 8;
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
}

// our own clear_image
void	clear_image(t_game *game)
{
	int	y;
	int	x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			put_pixel(x, y, 0, game);
			x++;
		}
		y++;
	}
}

// utils functions
void	draw_square(int x, int y, int size, int color, t_game *game)
{
	int	i;

	i = 0;
	while (i < size)
	{
		put_pixel(x + i, y, color, game);
		i++;
	}
	i = 0;
	while (i < size)
	{
		put_pixel(x, y + i, color, game);
		i++;
	}
	i = 0;
	while (i < size)
	{
		put_pixel(x + size, y + i, color, game);
		i++;
	}
	i = 0;
	while (i < size)
	{
		put_pixel(x + i, y + size, color, game);
		i++;
	}
}

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

// distance calculation functions
float	distance(float x, float y)
{
	return (sqrt(x * x + y * y));
}

// Helper for touch function
static bool	check_adjacent(t_game *game, int gridX, int gridY, float subX,
		float subY)
{
	if (subX < 0.05 && gridX > 0 && game->map[gridY][gridX - 1] == '1')
		return (true);
	if (subX > 0.95 && gridX < game->map_width - 1 && game->map[gridY][gridX
		+ 1] == '1')
		return (true);
	if (subY < 0.05 && gridY > 0 && game->map[gridY - 1][gridX] == '1')
		return (true);
	if (subY > 0.95 && gridY < game->map_height - 1 && game->map[gridY
		+ 1][gridX] == '1')
		return (true);
	return (false);
}

// touch function with bounds checking
bool	touch(float px, float py, t_game *game)
{
	int		gridX;
	int		gridY;
	float	subX;
	float	subY;

	gridX = (int)(px / BLOCK);
	gridY = (int)(py / BLOCK);
	if (gridY < 0 || gridY >= game->map_height || gridX < 0
		|| gridX >= game->map_width)
		return (true);
	if (game->map[gridY][gridX] == '1')
		return (true);
	subX = fmod(px, BLOCK) / BLOCK;
	subY = fmod(py, BLOCK) / BLOCK;
	return (check_adjacent(game, gridX, gridY, subX, subY));
}

void	init_game(t_game *game, t_map *map, t_parse *color)
{
	init_player(&game->player, map->player_dir, map);
	game->map = map->map;
	game->map_width = map->map_w;
	game->map_height = map->map_h;
	game->floor_color = get_color(color, 'F');
	game->ceiling_color = get_color(color, 'C');
	// Set texture paths (update with your actual paths)
	game->textures.no_path = "textures/wall_1.xpm";
	game->textures.so_path = "textures/wall_2.xpm";
	game->textures.we_path = "textures/wall_3.xpm";
	game->textures.ea_path = "textures/wall_4.xpm";
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line,
			&game->endian);
	// Load textures
	load_all_textures(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

int	draw_loop(t_game *game)
{
	t_player	*player;
	float		rayStart;
	float		rayStep;
	int			x;
	float		fov;
	
	fov = PI / 3;
	player = &game->player;
	move_player(player, game);
	clear_image(game);
	rayStart = player->angle - fov / 2;
	rayStep = fov / WIDTH;
	x = 0;
	while (x < WIDTH)
	{
		draw_line(player, game, rayStart, x);
		rayStart += rayStep;
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
