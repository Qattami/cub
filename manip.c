#include "parsing.h"

// Convert RGB to hexadecimal color
int rgb_to_hex(int r, int g, int b)
{
    if (r < 0) r = 0;
    else if (r > 255) r = 255;
    
    if (g < 0) g = 0;
    else if (g > 255) g = 255;
    
    if (b < 0) b = 0;
    else if (b > 255) b = 255;
    
    return (r << 16) | (g << 8) | b;
}

// Get color from parse list
int get_color(t_parse *color_list, char type)
{
    t_parse *current = color_list;
    while (current != NULL)
    {
        if (current->f_c == type) {
            return rgb_to_hex(current->r, current->g, current->b);
        }
        current = current->next;
    }
    return 0;
    
    // Default colors if not found
    // if (type == 'F') {
    //     return rgb_to_hex(139, 69, 19); // Brown floor
    // } else {
    //     return rgb_to_hex(135, 206, 235); // Blue ceiling
    // }
}

// our own put_pixel
void put_pixel(int x, int y, int color, t_game *game)
{
    if(x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return;
    
    int index = y * game->size_line + x * game->bpp / 8;
    game->data[index] = color & 0xFF;
    game->data[index + 1] = (color >> 8) & 0xFF;
    game->data[index + 2] = (color >> 16) & 0xFF;
}

// our own clear_image
void clear_image(t_game *game)
{
    for(int y = 0; y < HEIGHT; y++)
        for(int x = 0; x < WIDTH; x++)
            put_pixel(x, y, 0, game);
}

// utils functions
void draw_square(int x, int y, int size, int color, t_game *game)
{
    for(int i = 0; i < size; i++)
        put_pixel(x + i, y, color, game);
    for(int i = 0; i < size; i++)
        put_pixel(x, y + i, color, game);
    for(int i = 0; i < size; i++)
        put_pixel(x + size, y + i, color, game);
    for(int i = 0; i < size; i++)
        put_pixel(x + i, y + size, color, game);
}

void draw_map(t_game *game)
{
    char **map = game->map;
    int color = 0x0000FF;
    for(int y = 0; y < game->map_height; y++)
        for(int x = 0; x < game->map_width; x++)
            if(map[y][x] == '1')
                draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
}

// distance calculation functions
float distance(float x, float y){
    return sqrt(x * x + y * y);
}

// touch function with bounds checking
bool touch(float px, float py, t_game *game)
{
    int gridX = (int)(px / BLOCK);
    int gridY = (int)(py / BLOCK);
    
    // Bounds checking
    if (gridY < 0 || gridY >= game->map_height || gridX < 0 || gridX >= game->map_width)
        return true;
    
    // Check current cell
    if (game->map[gridY][gridX] == '1')
        return true;
    
    // Check adjacent cells for corner cases
    float subX = fmod(px, BLOCK) / BLOCK;
    float subY = fmod(py, BLOCK) / BLOCK;
    
    // Check if we're near a corner (within 5% of block edge)
    if (subX < 0.05) {
        if (gridX > 0 && game->map[gridY][gridX-1] == '1')
            return true;
    }
    if (subX > 0.95) {
        if (gridX < game->map_width-1 && game->map[gridY][gridX+1] == '1')
            return true;
    }
    if (subY < 0.05) {
        if (gridY > 0 && game->map[gridY-1][gridX] == '1')
            return true;
    }
    if (subY > 0.95) {
        if (gridY < game->map_height-1 && game->map[gridY+1][gridX] == '1')
            return true;
    }
    
    return false;
}

void init_game(t_game *game, t_map *map, t_parse *color)
{
    init_player(&game->player);
    game->map = map->map;
    game->map_width = map->map_width;
    game->map_height = map->map_height;
    game->floor_color = get_color(color, 'F');
    game->ceiling_color = get_color(color, 'C');
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

// DDA-based raycasting
void draw_line(t_player *player, t_game *game, float ray_angle, int column)
{
    float rayDirX = cos(ray_angle);
    float rayDirY = sin(ray_angle);
    
    // Player position in grid units
    float posX = player->x;
    float posY = player->y;
    
    // Grid coordinates
    int mapX = (int)(posX / BLOCK);
    int mapY = (int)(posY / BLOCK);
    
    // Direction vectors for ray stepping
    float deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
    float deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
    
    float sideDistX, sideDistY;
    int stepX, stepY;
    int hit = 0;
    int side; // 0 = x-side hit, 1 = y-side hit

    // Determine step direction and initial side distances
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (posX - mapX * BLOCK) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = ((mapX + 1) * BLOCK - posX) * deltaDistX;
    }
    
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (posY - mapY * BLOCK) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = ((mapY + 1) * BLOCK - posY) * deltaDistY;
    }

    // DDA Algorithm - detects walls at corners
    while (!hit) {
        // Jump to next map square
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX * BLOCK;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY * BLOCK;
            mapY += stepY;
            side = 1;
        }
        
        // Check for hit - add bounds checking
        if (mapY < 0 || mapX < 0 || mapY >= game->map_height || mapX >= game->map_width) {
            hit = 1; // Out of bounds
        } else if (game->map[mapY][mapX] == '1') {
            hit = 1; // Hit a wall
        }
        float lineHeight = (BLOCK * HEIGHT) / (hit + 0.0001f);
    if (lineHeight > HEIGHT) lineHeight = HEIGHT;
    
    int drawStart = (HEIGHT - lineHeight) / 2;
    int drawEnd = drawStart + lineHeight;
    
    // Draw the vertical line with depth shading
    int color = 255;
    if (side == 1) color = 200; // Darker for y-side walls
    
    for (int y = 0; y < HEIGHT; y++) {
        if (y < drawStart) {
            // Ceiling - use parsed color
            put_pixel(column, y, game->ceiling_color, game);
        } else if (y > drawEnd) {
            // Floor - use parsed color
            put_pixel(column, y, game->floor_color, game);
        } else {
            // Wall
            put_pixel(column, y, color, game);
        }
    }
    }

    // Calculate distance to hit point
    float perpDist;
    if (side == 0)
        perpDist = (sideDistX - deltaDistX * BLOCK);
    else
        perpDist = (sideDistY - deltaDistY * BLOCK);
    
    // Convert to pixel distance
    float dist = fabs(perpDist);

    // Debug mode: draw ray path
    if (DEBUG) {
        float rayX = posX;
        float rayY = posY;
        float stepSize = 0.5f;
        int steps = 0;
        
        while (!touch(rayX, rayY, game) && steps < 1000) {
            put_pixel(rayX, rayY, 0xFF0000, game);
            rayX += rayDirX * stepSize;
            rayY += rayDirY * stepSize;
            steps++;
        }
        return;
    }

    // Normal mode: draw 3D walls
    float lineHeight = (BLOCK * HEIGHT) / (dist + 0.0001f);
    if (lineHeight > HEIGHT) lineHeight = HEIGHT;
    
    int drawStart = (HEIGHT - lineHeight) / 2;
    int drawEnd = drawStart + lineHeight;
    
    // Draw the vertical line with depth shading
    int color = 255;
    if (side == 1) color = 200; // Darker for y-side walls
    
    for (int y = 0; y < HEIGHT; y++) {
        if (y < drawStart) {
            // Ceiling
            put_pixel(column, y, 0x87CEEB, game);
        } else if (y > drawEnd) {
            // Floor
            put_pixel(column, y, 0x8B4513, game);
        } else {
            // Wall
            put_pixel(column, y, color, game);
        }
    }
}

int draw_loop(t_game *game)
{
    t_player *player = &game->player;
    move_player(player, game);
    clear_image(game);

    // Debug mode: 2D view
    if (DEBUG) {
        draw_map(game);
        draw_square(player->x, player->y, 10, 0x00FF00, game);
    }

    // Cast rays for both modes
    float fov = PI / 3; // 60-degree FOV
    float rayStart = player->angle - fov / 2;
    float rayStep = fov / WIDTH;
    
    for (int x = 0; x < WIDTH; x++) {
        draw_line(player, game, rayStart, x);
        rayStart += rayStep;
    }
    
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return 0;
}