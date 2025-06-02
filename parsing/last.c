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
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			put_pixel(x, y, 0, game);
}

// utils functions
void	draw_square(int x, int y, int size, int color, t_game *game)
{
	for (int i = 0; i < size; i++)
		put_pixel(x + i, y, color, game);
	for (int i = 0; i < size; i++)
		put_pixel(x, y + i, color, game);
	for (int i = 0; i < size; i++)
		put_pixel(x + size, y + i, color, game);
	for (int i = 0; i < size; i++)
		put_pixel(x + i, y + size, color, game);
}

void	draw_map(t_game *game)
{
	char	**map;
	int		color;

	map = game->map;
	color = 0x0000FF;
	for (int y = 0; y < game->map_height; y++)
		for (int x = 0; x < game->map_width; x++)
			if (map[y][x] == '1')
				draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
}

// distance calculation functions
float	distance(float x, float y)
{
	return (sqrt(x * x + y * y));
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
	// Bounds checking
	if (gridY < 0 || gridY >= game->map_height || gridX < 0
		|| gridX >= game->map_width)
		return (true);
	// Check current cell
	if (game->map[gridY][gridX] == '1')
		return (true);
	// Check adjacent cells for corner cases
	subX = fmod(px, BLOCK) / BLOCK;
	subY = fmod(py, BLOCK) / BLOCK;
	// Check if we're near a corner (within 5% of block edge)
	if (subX < 0.05)
	{
		if (gridX > 0 && game->map[gridY][gridX - 1] == '1')
			return (true);
	}
	if (subX > 0.95)
	{
		if (gridX < game->map_width - 1 && game->map[gridY][gridX + 1] == '1')
			return (true);
	}
	if (subY < 0.05)
	{
		if (gridY > 0 && game->map[gridY - 1][gridX] == '1')
			return (true);
	}
	if (subY > 0.95)
	{
		if (gridY < game->map_height - 1 && game->map[gridY + 1][gridX] == '1')
			return (true);
	}
	return (false);
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

// DDA-based raycasting
void	draw_line(t_player *player, t_game *game, float ray_angle, int column)
{
	float			rayDirX;
	float			rayDirY;
	float			posX;
	float			posY;
	int				mapX;
	int				mapY;
	float			deltaDistX;
	float			deltaDistY;
	int				hit;
	float			perpDist;
	float			dist;
	float			rayX;
	float			rayY;
	float			stepSize;
	int				steps;
	float			lineHeight;
	int				drawStart;
	int				drawEnd;
	float			wallX;
	void			*texture_img;
	char			*texture_data;
	int				tex_width;
	int				tex_height;
	int				texX;
	double			step;
	double			texPos;
	int				texY;
	int				tex_index;
	unsigned char	b;
	unsigned char	g;
	unsigned char	r;
	int				color;

	rayDirX = cos(ray_angle);
	rayDirY = sin(ray_angle);
	posX = player->x;
	posY = player->y;
	mapX = (int)(posX / BLOCK);
	mapY = (int)(posY / BLOCK);
	// Calculate delta distances
	if (rayDirX == 0)
	{
		deltaDistX = 1e30;
	}
	else
	{
		deltaDistX = fabs(1 / rayDirX);
	}
	if (rayDirY == 0)
	{
		deltaDistY = 1e30;
	}
	else
	{
		deltaDistY = fabs(1 / rayDirY);
	}
	float sideDistX, sideDistY;
	int stepX, stepY;
	hit = 0;
	int side; // 0=x-side, 1=y-side
	// Determine step and initial side distances
	if (rayDirX < 0)
	{
		stepX = -1;
		sideDistX = (posX - mapX * BLOCK) * deltaDistX;
	}
	else
	{
		stepX = 1;
		sideDistX = ((mapX + 1) * BLOCK - posX) * deltaDistX;
	}
	if (rayDirY < 0)
	{
		stepY = -1;
		sideDistY = (posY - mapY * BLOCK) * deltaDistY;
	}
	else
	{
		stepY = 1;
		sideDistY = ((mapY + 1) * BLOCK - posY) * deltaDistY;
	}
	// DDA Algorithm
	while (hit == 0)
	{
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX * BLOCK;
			mapX += stepX;
			side = 0;
		}
		else
		{
			sideDistY += deltaDistY * BLOCK;
			mapY += stepY;
			side = 1;
		}
		// Check if ray hit a wall
		if (mapY < 0 || mapX < 0 || mapY >= game->map_height
			|| mapX >= game->map_width)
		{
			hit = 1;
		}
		else if (game->map[mapY][mapX] == '1')
		{
			hit = 1;
		}
	}
	// Calculate distance
	if (side == 0)
	{
		perpDist = (sideDistX - deltaDistX * BLOCK);
	}
	else
	{
		perpDist = (sideDistY - deltaDistY * BLOCK);
	}
	dist = fabs(perpDist);
	// Debug mode ray drawing
	if (DEBUG)
	{
		rayX = posX;
		rayY = posY;
		stepSize = 0.5f;
		steps = 0;
		while (!touch(rayX, rayY, game) && steps < 1000)
		{
			// Error: Could not get texture data\n
			put_pixel(rayX, rayY, 0xFF0000, game);
			rayX += rayDirX * stepSize;
			rayY += rayDirY * stepSize;
			steps++;
		}
		return ;
	}
	// Calculate line height
	lineHeight = (BLOCK * HEIGHT) / (dist + 0.0001f);
	if (lineHeight > HEIGHT)
	{
		lineHeight = HEIGHT;
	}
	drawStart = (HEIGHT - lineHeight) / 2;
	drawEnd = drawStart + lineHeight;
	// Ensure drawStart and drawEnd are within screen bounds
	if (drawStart < 0)
		drawStart = 0;
	if (drawEnd >= HEIGHT)
		drawEnd = HEIGHT - 1;
	// FIXED TEXTURE COORDINATE CALCULATION
	if (side == 0)
	{
		wallX = posY + perpDist * rayDirY;
	}
	else
	{
		wallX = posX + perpDist * rayDirX;
	}
	// Convert to texture coordinates properly - THIS IS THE KEY FIX
	// Map the hit position to 0.0-1.0 within the current block
	wallX = fmod(wallX, BLOCK) / BLOCK;
	if (wallX < 0)
		wallX += 1.0;
	// Select texture based on wall side and direction
	texture_img = NULL;
	if (side == 1)
	{ // Horizontal wall hit
		if (rayDirY > 0)
		{
			texture_img = game->textures.so_img; // South wall
		}
		else
		{
			texture_img = game->textures.no_img; // North wall
		}
	}
	else
	{ // Vertical wall hit
		if (rayDirX > 0)
		{
			texture_img = game->textures.ea_img; // East wall
		}
		else
		{
			texture_img = game->textures.we_img; // West wall
		}
	}
	// Get texture data
	int tex_bpp, tex_size_line, tex_endian;
	texture_data = mlx_get_data_addr(texture_img, &tex_bpp, &tex_size_line,
			&tex_endian);
	if (!texture_data)
	{
		printf("Error: Could not get texture data\n");
		return ;
	}
	// Use the texture dimensions from game structure
	tex_width = game->textures.tex_width;
	tex_height = game->textures.tex_height;
	// Calculate texture X coordinate with bounds checking
	texX = (int)(wallX * tex_width);
	if (texX >= tex_width)
		texX = tex_width - 1;
	if (texX < 0)
		texX = 0;
	// Fix the texture flipping logic
	if (side == 0 && rayDirX > 0)
	{
		texX = tex_width - texX - 1;
	}
	if (side == 1 && rayDirY < 0)
	{
		texX = tex_width - texX - 1;
	}
	// IMPROVED TEXTURE DRAWING WITH BETTER PRECISION
	step = (double)tex_height / lineHeight;
	texPos = (drawStart - HEIGHT / 2.0 + lineHeight / 2.0) * step;
	// Draw textured wall with bounds checking
	for (int y = drawStart; y < drawEnd; y++)
	{
		// Calculate current texture Y coordinate with bounds checking
		texY = (int)texPos;
		if (texY >= tex_height)
			texY = tex_height - 1;
		if (texY < 0)
			texY = 0;
		// Calculate texture index
		tex_index = texY * tex_size_line + texX * (tex_bpp / 8);
		// Bounds check for texture data access
		if (tex_index >= 0 && tex_index < tex_size_line * tex_height - 3)
		{
			// Read color safely (assuming BGRA format)
			b = texture_data[tex_index];
			g = texture_data[tex_index + 1];
			r = texture_data[tex_index + 2];
			color = (r << 16) | (g << 8) | b;
			// Apply shading for y-side walls (makes them slightly darker)
			if (side == 1)
			{
				color = ((color >> 1) & 0x7F7F7F);
			}
			put_pixel(column, y, color, game);
		}
		else
		{
			// Fallback color if texture access fails
			put_pixel(column, y, 0x808080, game); // Gray
		}
		texPos += step;
	}
	// Draw ceiling
	for (int y_ceil = 0; y_ceil < drawStart; y_ceil++)
	{
		put_pixel(column, y_ceil, game->ceiling_color, game);
	}
	// Draw floor
	for (int y_floor = drawEnd; y_floor < HEIGHT; y_floor++)
	{
		put_pixel(column, y_floor, game->floor_color, game);
	}
}

int	draw_loop(t_game *game)
{
	t_player *player = &game->player;
	move_player(player, game);
	clear_image(game);

	// Debug mode: 2D view
	if (DEBUG)
	{
		draw_map(game);
		draw_square(player->x, player->y, 10, 0x00FF00, game);
	}

	// Cast rays for both modes
	float fov = PI / 3; // 60-degree FOV
	float rayStart = player->angle - fov / 2;
	float rayStep = fov / WIDTH;

	for (int x = 0; x < WIDTH; x++)
	{
		draw_line(player, game, rayStart, x);
		rayStart += rayStep;
	}

	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}