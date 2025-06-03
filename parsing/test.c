#include "parsing.h"

void init_ray_direction(t_ray_data *ray, float ray_angle)
{
    ray->dirX = cos(ray_angle);
    ray->dirY = sin(ray_angle);
}

void init_ray_position(t_ray_data *ray, t_player *player)
{
    ray->posX = player->x;
    ray->posY = player->y;
    ray->mapX = (int)(ray->posX / BLOCK);
    ray->mapY = (int)(ray->posY / BLOCK);
}

void calculate_delta_distances(t_ray_data *ray)
{
    if (ray->dirX == 0) {
        ray->deltaDistX = 1e30;
    } else {
        ray->deltaDistX = fabs(1 / ray->dirX);
    }
    if (ray->dirY == 0) {
        ray->deltaDistY = 1e30;
    } else {
        ray->deltaDistY = fabs(1 / ray->dirY);
    }
}

void calculate_x_step(t_ray_data *ray)
{
    if (ray->dirX < 0) {
        ray->stepX = -1;
        ray->sideDistX = (ray->posX - ray->mapX * BLOCK) * ray->deltaDistX;
    } else {
        ray->stepX = 1;
        ray->sideDistX = ((ray->mapX + 1) * BLOCK - ray->posX) * ray->deltaDistX;
    }
}

void calculate_y_step(t_ray_data *ray)
{
    if (ray->dirY < 0) {
        ray->stepY = -1;
        ray->sideDistY = (ray->posY - ray->mapY * BLOCK) * ray->deltaDistY;
    } else {
        ray->stepY = 1;
        ray->sideDistY = ((ray->mapY + 1) * BLOCK - ray->posY) * ray->deltaDistY;
    }
}

int check_wall_hit(t_ray_data *ray, t_game *game)
{
    if (ray->mapY < 0 || ray->mapX < 0 || ray->mapY >= game->map_height || ray->mapX >= game->map_width) {
        return 1;
    }
    if (game->map[ray->mapY][ray->mapX] == '1') {
        return 1;
    }
    return 0;
}

void perform_dda_step(t_ray_data *ray)
{
    if (ray->sideDistX < ray->sideDistY) {
        ray->sideDistX += ray->deltaDistX * BLOCK;
        ray->mapX += ray->stepX;
        ray->side = 0;
    } else {
        ray->sideDistY += ray->deltaDistY * BLOCK;
        ray->mapY += ray->stepY;
        ray->side = 1;
    }
}

void run_dda_algorithm(t_ray_data *ray, t_game *game)
{
    int hit;
    hit = 0;
    while (hit == 0) {
        perform_dda_step(ray);
        hit = check_wall_hit(ray, game);
    }
}

void calculate_perpendicular_distance(t_ray_data *ray)
{
    if (ray->side == 0) {
        ray->perpDist = (ray->mapX * BLOCK - ray->posX + (1 - ray->stepX) / 2 * BLOCK) / ray->dirX;
    } else {
        ray->perpDist = (ray->mapY * BLOCK - ray->posY + (1 - ray->stepY) / 2 * BLOCK) / ray->dirY;
    }
}

void apply_fisheye_correction(t_ray_data *ray, t_player *player, float ray_angle)
{
    ray->distance = fabs(ray->perpDist * cos(ray_angle - player->angle));
    if (ray->distance < 0.1f) {
        ray->distance = 0.1f;
    }
}

void init_debug_ray_vars(float *rayX, float *rayY, float *stepSize, int *steps, t_ray_data *ray)
{
    *rayX = ray->posX;
    *rayY = ray->posY;
    *stepSize = 0.5f;
    *steps = 0;
}

void step_debug_ray(float *rayX, float *rayY, t_ray_data *ray, float stepSize)
{
    *rayX += ray->dirX * stepSize;
    *rayY += ray->dirY * stepSize;
}

void draw_debug_line(t_ray_data *ray, t_game *game)
{
    float rayX;
    float rayY;
    float stepSize;
    int steps;
    init_debug_ray_vars(&rayX, &rayY, &stepSize, &steps, ray);
    while (!touch(rayX, rayY, game) && steps < 1000) {
        put_pixel(rayX, rayY, 0xFF0000, game);
        step_debug_ray(&rayX, &rayY, ray, stepSize);
        steps++;
    }
}

void calculate_line_height(t_wall_data *wall, float distance)
{
    wall->lineHeight = (BLOCK * HEIGHT) / (distance + 0.01f);
    if (wall->lineHeight > HEIGHT * 2) {
        wall->lineHeight = HEIGHT * 2;
    }
}

void calculate_draw_bounds(t_wall_data *wall)
{
    wall->drawStart = (HEIGHT - (int)wall->lineHeight) / 2;
    wall->drawEnd = wall->drawStart + (int)wall->lineHeight;
    if (wall->drawStart < 0) {
        wall->drawStart = 0;
    }
    if (wall->drawEnd >= HEIGHT) {
        wall->drawEnd = HEIGHT - 1;
    }
}

void calculate_wall_x_coordinate(t_wall_data *wall, t_ray_data *ray)
{
    if (ray->side == 0) {
        wall->wallX = ray->posY + ray->perpDist * ray->dirY;
    } else {
        wall->wallX = ray->posX + ray->perpDist * ray->dirX;
    }
    wall->wallX = wall->wallX - floor(wall->wallX / BLOCK) * BLOCK;
    wall->wallX = wall->wallX / BLOCK;
    if (wall->wallX < 0.0f) {
        wall->wallX = 0.0f;
    }
    if (wall->wallX > 1.0f) {
        wall->wallX = 1.0f;
    }
}

void select_wall_texture(t_wall_data *wall, t_ray_data *ray, t_game *game)
{
    if (ray->side == 1) {
        if (ray->dirY > 0) {
            wall->texture_img = game->textures.so_img;
        } else {
            wall->texture_img = game->textures.no_img;
        }
    } else {
        if (ray->dirX > 0) {
            wall->texture_img = game->textures.ea_img;
        } else {
            wall->texture_img = game->textures.we_img;
        }
    }
}

void calculate_texture_x_coordinate(t_wall_data *wall, int tex_width)
{
    wall->texX = (int)(wall->wallX * tex_width);
    if (wall->texX >= tex_width) {
        wall->texX = tex_width - 1;
    }
    if (wall->texX < 0) {
        wall->texX = 0;
    }
}

void apply_texture_flipping(t_wall_data *wall, t_ray_data *ray, int tex_width)
{
    if (ray->side == 0 && ray->dirX > 0) {
        wall->texX = tex_width - wall->texX - 1;
    }
    if (ray->side == 1 && ray->dirY < 0) {
        wall->texX = tex_width - wall->texX - 1;
    }
}

int get_texture_data(t_texture_data *tex, void *img, t_game *game)
{
    int endian;
    tex->data = mlx_get_data_addr(img, &tex->bpp, &tex->size_line, &endian);
    if (!tex->data) {
        return 0;
    }
    tex->width = game->textures.tex_width;
    tex->height = game->textures.tex_height;
    return 1;
}

void extract_rgb_components(unsigned char *b, unsigned char *g, unsigned char *r, t_texture_data *tex, int tex_index)
{
    *b = tex->data[tex_index];
    *g = tex->data[tex_index + 1];
    *r = tex->data[tex_index + 2];
}

int apply_side_shading(int color, int side)
{
    if (side == 1) {
        return ((color >> 1) & 0x7F7F7F);
    }
    return color;
}

int apply_distance_fade(int color, float distance)
{
    float fade;
    int r_comp;
    int g_comp;
    int b_comp;
    if (distance >= 5.0f) {
        return color;
    }
    fade = distance / 5.0f;
    r_comp = ((color >> 16) & 0xFF) * fade;
    g_comp = ((color >> 8) & 0xFF) * fade;
    b_comp = (color & 0xFF) * fade;
    return (r_comp << 16) | (g_comp << 8) | b_comp;
}

int calculate_color_with_shading(t_texture_data *tex, int tex_index, int side, float distance)
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
    int color;
    extract_rgb_components(&b, &g, &r, tex, tex_index);
    color = (r << 16) | (g << 8) | b;
    color = apply_side_shading(color, side);
    color = apply_distance_fade(color, distance);
    return color;
}

void init_texture_step_values(double *step, double *texPos, t_wall_data *wall, t_texture_data *tex)
{
    *step = (double)tex->height / wall->lineHeight;
    *texPos = (wall->drawStart - HEIGHT/2.0 + wall->lineHeight/2.0) * *step;
    if (*texPos < 0) {
        *texPos = 0;
    }
}

int calculate_texture_y(double texPos, int tex_height)
{
    int texY;
    texY = (int)texPos;
    if (texY >= tex_height) {
        texY = tex_height - 1;
    }
    if (texY < 0) {
        texY = 0;
    }
    return texY;
}

int get_safe_texture_index(int texY, int texX, t_texture_data *tex)
{
    int tex_index;
    tex_index = texY * tex->size_line + texX * (tex->bpp / 8);
    if (tex_index >= 0 && tex_index < (tex->size_line * tex->height - 4)) {
        return tex_index;
    }
    return -1;
}

void draw_wall_column(t_wall_data *wall, t_texture_data *tex, t_ray_data *ray, int column, t_game *game)
{
    double step;
    double texPos;
    int y;
    int texY;
    int tex_index;
    init_texture_step_values(&step, &texPos, wall, tex);
    y = wall->drawStart;
    while (y < wall->drawEnd && y < HEIGHT) {
        texY = calculate_texture_y(texPos, tex->height);
        tex_index = get_safe_texture_index(texY, wall->texX, tex);
        if (tex_index != -1) {
            int color = calculate_color_with_shading(tex, tex_index, ray->side, ray->distance);
            put_pixel(column, y, color, game);
        } else {
            put_pixel(column, y, 0x808080, game);
        }
        texPos += step;
        y++;
    }
}

void draw_ceiling(t_wall_data *wall, int column, t_game *game)
{
    int y;
    y = 0;
    while (y < wall->drawStart) {
        put_pixel(column, y, game->ceiling_color, game);
        y++;
    }
}

void draw_floor(t_wall_data *wall, int column, t_game *game)
{
    int y;
    y = wall->drawEnd;
    while (y < HEIGHT) {
        put_pixel(column, y, game->floor_color, game);
        y++;
    }
}

void draw_line(t_player *player, t_game *game, float ray_angle, int column)
{
    t_ray_data ray;
    t_wall_data wall;
    t_texture_data tex;
    init_ray_direction(&ray, ray_angle);
    init_ray_position(&ray, player);
    calculate_delta_distances(&ray);
    calculate_x_step(&ray);
    calculate_y_step(&ray);
    run_dda_algorithm(&ray, game);
    calculate_perpendicular_distance(&ray);
    apply_fisheye_correction(&ray, player, ray_angle);
    calculate_line_height(&wall, ray.distance);
    calculate_draw_bounds(&wall);
    calculate_wall_x_coordinate(&wall, &ray);
    select_wall_texture(&wall, &ray, game);
    if (!get_texture_data(&tex, wall.texture_img, game)) {
        printf("Error: Could not get texture data\n");
        return;
    }
    calculate_texture_x_coordinate(&wall, tex.width);
    apply_texture_flipping(&wall, &ray, tex.width);
    draw_wall_column(&wall, &tex, &ray, column, game);
    draw_ceiling(&wall, column, game);
    draw_floor(&wall, column, game);
}
