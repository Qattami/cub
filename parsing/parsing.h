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

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
// #include "../src/mlx/mlx.h"
//parsing

typedef struct s_parse
{
    char f_c;
    int r;
    int g;
    int b;
    struct s_parse *next;
}t_parse;

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
    	int map_width;
	int map_height;
} t_game;

void init_player(t_player *player);
int key_release(int keycode, t_player *player);
int key_press(int keycode, t_player *player);
void move_player(t_player *player);

int store_RGB(char *c, char *s, t_parse *d);
void free_tab(char **t);
int get_f_c(t_parse *d, char *s);
char **get_6_lines(void);
int pars_extension(char *s, char *s2);
int extension(char *s1, char **tab);
int valid_trend(char **t);
char **get_map_lines(int fd);
void save_map_to_struct(t_game *data, char **map_lines);
int is_map_surrounded(t_game *data);

//libft

char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *str, char charset);
int	ft_isdigit(int c);
int	ft_atoi(const char *str);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
#endif


