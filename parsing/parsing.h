#ifndef PARSING_H
#define PARSING_H

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

#include "get_next_line.h"
#include "../mlx/mlx.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//parsing

typedef struct s_parse
{
    char f_c;
    int r;
    int g;
    int b;
    struct s_parse *next;
}t_parse;

typedef struct s_map
{
	char **map;
	int map_width;
	int map_height;
}	t_map;

typedef struct s_player
{
    float x;
    float y;
    float angle;

    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;

    bool left_rotate;
    bool right_rotate;
}   t_player;

typedef struct s_game
{
    void *mlx;
    void *win;
    void *img;

    char *data;
    int bpp;
    int size_line;
    int endian;
    t_player player;
    char **map;
} t_game;

int store_RGB(char *c, char *s, t_parse *d);
void free_tab(char **t);
int get_f_c(t_parse *d);
char **get_6_lines(void);
t_map *get_data(char *s, t_map *data);
int valid_trend(char **t);

//libft

char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *str, char charset);
int	ft_isdigit(int c);
int	ft_atoi(const char *str);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void init_player(t_player *player);
int key_release(int keycode, t_player *player);
int key_press(int keycode, t_player *player);
void move_player(t_player *player);
int draw_loop(t_game *game);
void draw_line(t_player *player, t_game *game, float start_x, int i);
void init_game(t_game *game, t_map *map);
bool touch(float px, float py, t_game *game);
float fixed_dist(float x1, float y1, float x2, float y2, t_game *game);
float distance(float x, float y);
void draw_map(t_game *game);
void draw_square(int x, int y, int size, int color, t_game *game);
void clear_image(t_game *game);
void put_pixel(int x, int y, int color, t_game *game);

#endif