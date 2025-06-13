/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:51:37 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/13 16:51:39 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# define WIDTH 1280
# define HEIGHT 720
# define BLOCK 64
# define DEBUG 0

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363

# define PI 3.14159265359

# define ESC 65307

# include "../mlx/mlx.h"
# include "get_next_line.h"
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_ray_data
{
	float			dir_x;
	float			dir_y;
	float			pos_x;
	float			pos_y;
	int				map_x;
	int				map_y;
	float			delta_distx;
	float			delta_disty;
	float			side_distx;
	float			side_disty;
	int				step_x;
	int				step_y;
	int				side;
	float			perp_dist;
	float			distance;
}					t_ray_data;

typedef struct s_wall_data
{
	float			wall_x;
	int				tex_x;
	float			line_height;
	int				draw_start;
	int				draw_end;
	void			*texture_img;
}					t_wall_data;

typedef struct s_texture_data
{
	char			*data;
	int				bpp;
	int				size_line;
	int				width;
	int				height;
}					t_texture_data;

typedef struct s_parse
{
	char			f_c;
	int				r;
	int				g;
	int				b;
	struct s_parse	*next;
}					t_parse;

typedef struct s_map
{
	int				i;
	int				j;
	int				count;
	char			c;
	char			**map;
	int				map_w;
	int				map_h;
	int				player_x;
	int				player_y;
	char			player_dir;
}					t_map;

typedef struct s_textures
{
	void			*no_img;
	void			*so_img;
	void			*we_img;
	void			*ea_img;
	int				tex_width;
	int				tex_height;
	char			*no_path;
	char			*so_path;
	char			*we_path;
	char			*ea_path;
}					t_textures;

typedef struct s_player
{
	float			x;
	float			y;
	float			angle;
	char			direction;

	bool			key_up;
	bool			key_down;
	bool			key_left;
	bool			key_right;

	bool			left_rotate;
	bool			right_rotate;
}					t_player;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	void			*img;

	char			*data;
	int				bpp;
	int				size_line;
	int				endian;
	t_player		player;
	t_textures		textures;
	char			**map;
	int				map_width;
	int				map_height;
	int				floor_color;
	int				ceiling_color;
}					t_game;

typedef struct s_render_context
{
	t_ray_data		*ray;
	t_wall_data		*wall;
	t_texture_data	*tex;
	t_game			*game;
}					t_render_context;

int					store_rgb(char *c, char *s, t_parse *d);
void				free_tab(char **t);
int					get_f_c(t_parse *d, t_game **game);
char				**get_6_lines(void);
t_map				*get_data(char *s, t_map *data);
int					valid_trend(char **t);
void				init_var(t_map *data);
int					end_of_read_lines(char *line, char **t);
void				free_map_data(t_map *data);
t_parse				*get_pars(t_parse *pars, t_game *game);
char				*helper31(char *line, int k);
// libft

char				*ft_strjoin(char const *s1, char const *s2);
char				**ft_split(char const *str, char charset);
int					ft_isdigit(int c);
int					ft_atoi(const char *str);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
void				*ft_memset(void *s, int c, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				init_player(t_player *player, char direction, t_map *map);
int					key_release(int keycode, t_player *player);
int					key_press(int keycode, t_player *player);
void				move_player(t_player *player, t_game *game);
int					draw_loop(t_game *game);
void				init_game(t_game *game, t_map *map, t_parse *color);
bool				touch(float px, float py, t_game *game);
float				fixed_dist(float x1, float y1, float x2, float y2,
						t_game *game);
float				distance(float x, float y);
void				draw_map(t_game *game);
void				draw_square(int x, int y, int size, int color,
						t_game *game);
void				clear_image(t_game *game);
void				put_pixel(int x, int y, int color, t_game *game);
int					rgb_to_hex(int r, int g, int b);
int					get_color(t_parse *color_list, char type);
void				load_texture(t_game *game, void **img, char *path);
void				load_all_textures(t_game *game);
void				draw_line(t_player *player, t_game *game, float ray_angle,
						int column);
void				draw_wall_column(t_render_context *ctx, int column);
int					calculate_color_with_shading(t_texture_data *tex,
						int tex_index, int side, float distance);
void				calculate_y_step(t_ray_data *ray);
int					check_wall_hit(t_ray_data *ray, t_game *game);
void				perform_dda_step(t_ray_data *ray);
void				run_dda_algorithm(t_ray_data *ray, t_game *game);
void				calculate_perpendicular_distance(t_ray_data *ray);
bool				check_adjacent(t_game *game, int gridX, int gridY,
						float subX, float subY);
bool				touch(float px, float py, t_game *game);
void				init_game(t_game *game, t_map *map, t_parse *color);
void				init_ray_direction(t_ray_data *ray, float ray_angle);
void				init_ray_position(t_ray_data *ray, t_player *player);
void				calculate_delta_distances(t_ray_data *ray);
void				calculate_x_step(t_ray_data *ray);
void				apply_fisheye_correction(t_ray_data *ray, t_player *player,
						float ray_angle);
void				init_debug_ray_vars(float *rayX, float *rayY,
						float *stepSize, int *steps, t_ray_data *ray);
void				step_debug_ray(float *rayX, float *rayY, t_ray_data *ray,
						float stepSize);
void				draw_debug_line(t_ray_data *ray, t_game *game);
void				calculate_line_height(t_wall_data *wall, float distance);
void				calculate_draw_bounds(t_wall_data *wall);
void				calculate_wall_x_coordinate(t_wall_data *wall,
						t_ray_data *ray);
void				select_wall_texture(t_wall_data *wall, t_ray_data *ray,
						t_game *game);
void				calculate_texture_x_coordinate(t_wall_data *wall,
						int tex_width);
void				apply_texture_flipping(t_wall_data *wall, t_ray_data *ray,
						int tex_width);
int					get_texture_data(t_texture_data *tex, void *img,
						t_game *game);
void				extract_rgb_components(unsigned char *b, unsigned char *g,
						unsigned char *r, t_texture_data *tex, int tex_index);
int					apply_side_shading(int color, int side);
int					apply_distance_fade(int color, float distance);
int					calculate_color_with_shading(t_texture_data *tex,
						int tex_index, int side, float distance);
void				init_texture_step_values(double *step, double *texPos,
						t_wall_data *wall, t_texture_data *tex);
int					calculate_texture_y(double texPos, int tex_height);
int					get_safe_texture_index(int texY, int texX,
						t_texture_data *tex);
void				setup_texture_step(t_wall_data *wall, t_texture_data *tex,
						double *step, double *texPos);
void				draw_wall_pixel(t_render_context *ctx, int column, int y,
						double texPos);
void				setup_ray_data(t_ray_data *ray, t_player *player,
						float ray_angle);
void				perform_wall_calculations(t_wall_data *wall,
						t_ray_data *ray, t_game *game);
void				draw_ceiling(t_wall_data *wall, int column, t_game *game);
void				draw_floor(t_wall_data *wall, int column, t_game *game);
void				save_map_to_struct(t_map *data, char **map_lines);
int					double_char(t_map *d);
int					is_map_surrounded(t_map *d);

#endif