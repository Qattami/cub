#include "parsing.h"

int pars_extension(char *s, char *s2)
{
    int len;
    int i;
    int j;
    j = 3;

    len = ft_strlen(s);
    len--;
    i = len - 3;
    while(len >= i)
    {        
        if(s[len] != s2[j])
        {  
            return 1;
        }
        else
        {
            j--;
            len--;
        }
    }
        return 0;
}

void	ft_bzero(int *s, size_t size)
{
	
	while (size > 0)
	{
		*s = 0;
		s++;
		size--;
	}
}

char **get_6_lines(void)
{
    int fd, i = 0;
    char **t;
    char *line;

    t = malloc(sizeof(char *) * 7); 
    if (!t)
        return NULL;
    fd = open("../maps/map.cub", O_RDONLY);
    if (fd < 0)
        return NULL;

    while ((line = get_next_line(fd)))
    {
        int k = 0;
        while (line[k] == ' ' || line[k] == '\t')
            k++;
        if (ft_strncmp(&line[k], "NO ", 3) == 0 || ft_strncmp(&line[k], "SO ", 3) == 0 ||
            ft_strncmp(&line[k], "WE ", 3) == 0 || ft_strncmp(&line[k], "EA ", 3) == 0 ||
            ft_strncmp(&line[k], "F ", 2) == 0 || ft_strncmp(&line[k], "C ", 2) == 0)
        {
            if (line[ft_strlen(line) - 1] == '\n')
                line[ft_strlen(line) - 1] = '\0';
            t[i++] = ft_strdup(&line[k]);
        }
        free(line);
        if (i == 6)
            break;
    }
    t[i] = NULL;
    close(fd);
    return t;
}



int existe(int *t)
{
    int i;
    
    i = 0;
    while(i < 5)
    {
        if(t[i] != 1)
            return 1;
        i++;
    }
    return 0 ;
}
int valid_trend(char **t)
{
    int i;
    int tl[5];
    ft_bzero(tl, 5);

    i = 0;
   
    while(i < 6)
    {
        if(t[i][0] == 'S' && t[i][1] == 'O' )
            tl[0] = 1;
        else if(t[i][0] == 'N' && t[i][1] == 'O' )
            tl[1] = 1;
        else if(t[i][0] == 'W' && t[i][1] == 'E' )
            tl[2] = 1;
        else if(t[i][0] == 'E' && t[i][1] == 'A' )
            tl[3] = 1;
        else if((!ft_strncmp(t[i], "C ", 2) && !ft_strncmp(t[i + 1] ,"F ", 2)) || (!ft_strncmp(t[i], "F ", 2) && !ft_strncmp(t[i + 1] ,"C ", 2)))
            {   
                printf("TANIYAaaaaaa\n");

                tl[4] = 1;
            }
         
        i++;
    }
    i = 0;
    
    while(i < 5)
        printf("%d\n", tl[i++]);
    return(existe(tl));
}
t_parse *get_pars(t_parse *pars)
{
    pars = malloc(sizeof(t_parse));
    if (!pars)
        return (NULL);
    pars->next = malloc(sizeof(t_parse));
    if (!pars->next)
    {
        free(pars);
        return NULL;
    }
    if(!get_f_c(pars))
        printf("RGB stored: R=%d G=%d B=%d\n", pars->r, pars->g, pars->b);
    printf("RGB stored: R=%d G=%d B=%d\n", pars->next->r, pars->next->g, pars->next->b);
    return(pars);
}
int main(int ac, char **av)
{
    (void) ac;
    t_parse *pars;
    t_game game;
    t_map *da;
	da = NULL;
    pars = NULL;
    pars = get_pars(pars);
    if(!pars)
    {
        free(pars);
        free(pars->next);
        return(write(1, "ERROR\n", 6), 1);
    }
	da = get_data(av[1], da);
	if(!da)
    {
        free_tab(da->map);
        free(da);
        return(write(1, "ERROR1\n", 7), 1);
    }
    init_game(&game, da);
    mlx_hook(game.win, 2, 1L<<0, key_press, &game.player);
    mlx_hook(game.win, 3, 1L<<1, key_release, &game.player);
    mlx_loop_hook(game.mlx, draw_loop, &game);
    mlx_loop(game.mlx);
	free_tab(da->map);
    free(da);
    free(pars);
    free(pars->next);
    return 0;
}
// int main(void)
// {
//     t_game game;
//     // init
//     init_game(&game);
//     // hooks
//     mlx_hook(game.win, 2, 1L<<0, key_press, &game.player);
//     mlx_hook(game.win, 3, 1L<<1, key_release, &game.player);
//     // draw loop
//     mlx_loop_hook(game.mlx, draw_loop, &game);

//     mlx_loop(game.mlx);
//     return 0;
// }