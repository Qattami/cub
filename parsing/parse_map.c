#include "parsing.h"

int is_map_line(char *line)
{
	while (*line == ' ' || *line == '\t')
		line++;
	return (*line == '1' || *line == '0'); 
}

char **get_map_lines(int fd)
{
	char **map = malloc(sizeof(char *) * 1024); 
	char *line;
	int i = 0, map_started = 0;

	while ((line = get_next_line(fd)))
	{
		if (!map_started)
		{
			if (line[0] == '\n' || ft_strlen(line) <= 1)
				;
			else if (ft_strncmp(line, "NO ", 3) == 0 ||
					 ft_strncmp(line, "SO ", 3) == 0 ||
					 ft_strncmp(line, "WE ", 3) == 0 ||
					 ft_strncmp(line, "EA ", 3) == 0 ||
					 ft_strncmp(line, "F ", 2) == 0 ||
					 ft_strncmp(line, "C ", 2) == 0)
				; 
			else if (is_map_line(line))
				map_started = 1;
			else
				continue;
		}
		if (map_started)
			map[i++] = ft_strdup(line);
		free(line);
	}
	map[i] = NULL;
	return map;
}

void save_map_to_struct(t_game *data, char **map_lines)
{
	int i , max_width, len, j;

    i = 0;
    max_width = 0;
	while (map_lines[i])
	{
		len = ft_strlen(map_lines[i]);
		if (len > 0 && map_lines[i][len - 1] == '\n')
			map_lines[i][len - 1] = '\0';
		if ((int)ft_strlen(map_lines[i]) > max_width)
			max_width = ft_strlen(map_lines[i]);
		i++;
	}
	data->map_height = i;
	data->map_width = max_width;
	data->map = malloc(sizeof(char *) * (i + 1));
    j = 0;
	while (j < i)
    {
	    data->map[j] = malloc(max_width + 1);
	    if (!data->map[j])
	    	return;
	    ft_memset(data->map[j], ' ', max_width); 
	    ft_memcpy(data->map[j], map_lines[j], ft_strlen(map_lines[j])); 
	    data->map[j][max_width] = '\0';
	    j++;
    }
	data->map[i] = NULL;
}
int	is_map_surrounded(t_game *game)
{
	int	i, j;
	char	**map = game->map;

	for (i = 0; i < game->map_height; i++)
	{
		for (j = 0; j < game->map_width; j++)
		{
			char c = map[i][j];
			if (c == '0' || ft_strchr("NSEW", c))
			{
				// Si on est en bord de map ou si voisin inexistant ou ' '
				if (
					i == 0 || j == 0 ||
					i >= game->map_height - 1 || j >= game->map_width - 1 ||
					map[i - 1][j] == ' ' || map[i + 1][j] == ' ' ||
					map[i][j - 1] == ' ' || map[i][j + 1] == ' ' ||
					map[i - 1][j] == '\0' || map[i + 1][j] == '\0' ||
					map[i][j - 1] == '\0' || map[i][j + 1] == '\0'
				)
					return (0);
			}
		}
	}
	return (1);
}



// int main(void)
// {
// 	int fd = open("../maps/map.cub", O_RDONLY);
// 	if (fd < 0)
// 		return (perror("open failed"), 1);

// 	t_game *data = malloc(sizeof(t_map));
// 	if (!data)
// 		return (perror("malloc failed"), 1);

// 	char **map_lines = get_map_lines(fd);
// 	if (!map_lines)
// 		return (printf("Failed to read map\n"), 1);

// 	save_map_to_struct(data, map_lines);
// 	free_tab(map_lines);

// 	printf("Map size: %dx%d\n", data->map_width, data->map_height);


// 	if (!is_map_surrounded(data))
// 		printf("Map not closed by walls!\n");
// 	else
// 		printf("Map is valid.\n");
// 	int i = 0, j;
// 	while (data->map[i])
// 	{
// 		j = 0;
// 		while (data->map[i][j])
// 		{
// 			printf("%c", data->map[i][j]);
// 			j++;
// 		}
// 		printf("\n");
// 		i++;
// 	}
// 	printf("Done\n");

// 	free_tab(data->map);
// 	free(data);
// 	return 0;
// }

