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
    int fd;
    int i;
    char **t;
    char *line;

    i = 0;
    t = malloc(sizeof(char*) * 7);
    if (!t)
        return NULL;

    fd = open("../maps/map.cub", O_RDONLY);
    if (fd < 0)
        return NULL;

    while(1)
    {
        line = get_next_line(fd);
        if (!line)
            break;
        if (line[0] != '\n')
        {
           if (line[ft_strlen(line) - 1] == '\n')
                line[ft_strlen(line) - 1] = '\0';  // Remove trailing newline
            t[i] = ft_strdup(line);
            i++;
        }
        free(line);
        if (i == 6)
            break;
    }
    close(fd);
    t[6] = NULL;
    return t;
}


int existe(int *t)
{
    int i;

    i = 0;
    while(i < 6)
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
    int tl[6];
    ft_bzero(tl, 6);

    i = 0;
   
    while(i < 6)
    {
        if(ft_strncmp(t[i], "SO ", 3) == 0 )
            tl[0] = 1;
        else if(ft_strncmp(t[i], "NO ", 3) == 0  )
            tl[1] = 1;
        else if(ft_strncmp(t[i], "WE ", 3) == 0 )
            tl[2] = 1;
        else if(ft_strncmp(t[i], "EA ", 3) == 0 )
            tl[3] = 1;
        else if(ft_strncmp(t[i], "C ", 2) == 0 )
            tl[4] = 1;
        else if(ft_strncmp(t[i], "F ", 2) == 0)
            tl[5] = 1;
        i++;
    }
    return(existe(tl));
}
int main()
{
    t_parse *data;
    data = malloc(sizeof(t_parse));
    data->next = malloc(sizeof(t_parse));
    if(!get_f_c(data, "../maps/map.cub"))
        printf("RGB stored===>: R=%d G=%d B=%d\n", data->r, data->g, data->b);    
    int fd = open("../maps/map.cub", O_RDONLY);
	if (fd < 0)
    {
        return (perror("open failed"), 1);
    }
	printf("%d\n", fd);

	t_game *map = malloc(sizeof(t_game));
	if (!map)
		return (perror("malloc failed"), 1);

	char **map_lines = get_map_lines(fd);
    int i = 0, j;
    while(map_lines[i])
    {
        j = 0;
        while(map_lines[i][j])
            printf("%c", map_lines[i][j++]);
        i++;
        printf("\n");
    }
	if (!map_lines)
		return (printf("Failed to read map\n"), 1);

	save_map_to_struct(map, map_lines);
	free_tab(map_lines);

	printf("Map size: %dx%d\n", map->map_width, map->map_height);


	if (!is_map_surrounded(map))
		printf("Map not closed by walls!\n");
	else
		printf("Map is valid.\n");
	// int i = 0, j;
	// while (map->map[i])
	// {
	// 	j = 0;
	// 	while (map->map[i][j])
	// 	{
	// 		printf("%c", map->map[i][j]);
	// 		j++;
	// 	}
	// 	printf("\n");
	// 	i++;
	// }
    close(fd);
	free_tab(map->map);
    free(data->next);
    free(data);
    free(map);
    return 0;
}
