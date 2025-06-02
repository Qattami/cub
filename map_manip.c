// #include "game.h"
#include "parsing.h"

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
    for(int y = 0; map[y]; y++)
        for(int x = 0; map[y][x]; x++)
            if(map[y][x] == '1')
                draw_square(x * BLOCK, y * BLOCK, BLOCK, color, game);
}

// distance calculation functions
float distance(float x, float y){
    return sqrt(x * x + y * y);
}

float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    float angle = atan2(delta_y, delta_x) - game->player.angle;
    float fix_dist = distance(delta_x, delta_y) * cos(angle);
    return fix_dist;
}

// touch function 
bool touch(float px, float py, t_game *game)
{
    // int x = px / BLOCK;
    // int y = py / BLOCK;
    // if(game->map[y][x] == '1')
    //     return true;
    // return false;
    int x = px / BLOCK;
    int y = py / BLOCK;
    
    // Bounds checking
    if (y < 0 || y >= game->map_height || x < 0 || x >= game->map_width)
        return true;
        
    return (game->map[y][x] == '1');
}

void init_game(t_game *game, t_map *map)
{
    init_player(&game->player);
    game->map_height = map->map_height;
    game->map_width = map->map_width;
    game->map = map->map;
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

// raycasting functions
void draw_line(t_player *player, t_game *game, float start_x, int i)
{
    // float cos_angle = cos(start_x);
    // float sin_angle = sin(start_x);
    // float ray_x = player->x;
    // float ray_y = player->y;

    // while(!touch(ray_x, ray_y, game))
    // {
    //     if(DEBUG)
    //         put_pixel(ray_x, ray_y, 0xFF0000, game);
    //     ray_x += cos_angle;
    //     ray_y += sin_angle;
    // }
    // if(!DEBUG)
    // {
    //     float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
    //     float height = (BLOCK / dist) * (WIDTH / 2);
    //     int start_y = (HEIGHT - height) / 2;
    //     int end = start_y + height;
    //     while(start_y < end)
    //     {
    //         put_pixel(i, start_y, 255, game);
    //         start_y++;
    //     }
    // }
        float rayDirX = cos(start_x);
    float rayDirY = sin(start_x);
    
    // Convert player position to grid coordinates
    float posX = player->x / BLOCK;
    float posY = player->y / BLOCK;
    
    // Grid coordinates
    int mapX = (int)posX;
    int mapY = (int)posY;
    
    // Length of ray from one side to next in grid
    float deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
    float deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
    
    float sideDistX, sideDistY;
    int stepX, stepY;
    int hit = 0;
    int side; // 0 = x-side, 1 = y-side hit

    // Calculate step and initial sideDist
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }

    // DDA algorithm
    while (!hit) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        
        // Check if ray has hit a wall
        if (mapY < 0 || mapX < 0 || mapY >= game->map_height || mapX >= game->map_width) {
            hit = 1; // Out of bounds
        } else if (game->map[mapY][mapX] == '1') {
            hit = 1;
        }
    }

    // Calculate distance projected on camera direction
    float perpWallDist;
    if (side == 0)
        perpWallDist = (sideDistX - deltaDistX);
    else
        perpWallDist = (sideDistY - deltaDistY);

    // Convert to world units
    float dist = perpWallDist * BLOCK;

    // Calculate line height and draw
    float height = (BLOCK / dist) * (WIDTH / 2);
    int start_y = (HEIGHT - height) / 2;
    int end_y = start_y + height;
    
    for (int y = start_y; y < end_y; y++) {
        put_pixel(i, y, 255, game);
    }
}

int draw_loop(t_game *game)
{
    t_player *player = &game->player;
    move_player(player, game);
    clear_image(game);
    if(DEBUG)
    {
        draw_square(player->x, player->y, 10, 0x00FF00, game);
        draw_map(game);
    }
    float fraction = PI / 3 / WIDTH;
    float start_x = player->angle - PI / 6;
    int i = 0;
    while(i < WIDTH)
    {
        draw_line(player, game, start_x, i);
        start_x += fraction;
        i++;
    }
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return 0;
}