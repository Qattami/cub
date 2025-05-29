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

void save_map_to_struct(t_map *data, char **map_lines)
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
int is_valid_tile(char c)
{
    return (c == '0' || ft_strchr("NSEW", c));
}

int is_valid_char(char c)
{
    return (c == '0' || c == '1' || c == ' ' || ft_strchr("NSEW", c));
}

int is_map_surrounded(t_map *data)
{
    int i;
	int j;

    for (i = 0; i < data->map_height; i++)
    {
        for (j = 0; j < data->map_width; j++)
        {
            char c = data->map[i][j];
            if (!is_valid_char(c))
                return (write(1, "Invalid character\n", 18), 0);
            if (is_valid_tile(c))
            {
                if (i == 0 || j == 0 || i == data->map_height - 1 || j == data->map_width - 1)
                    return (write(1, "Open edge\n", 10),0);
                if (data->map[i - 1][j] == ' ' || data->map[i + 1][j] == ' ' ||
                    data->map[i][j - 1] == ' ' || data->map[i][j + 1] == ' ')
                    return (write(1, "Open space\n", 11), 0);
            }
        }
    }
    return 1;
}
int double_char(t_map *data)
{
	int i = 0, j;
	int count = 0;

	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			if (data->map[i][j] == 'N' || data->map[i][j] == 'S' ||
				data->map[i][j] == 'E' || data->map[i][j] == 'W')
				count++;
			j++;
		}
		i++;
	}
	if (count != 1)
		return(write(1, "Invalid number of direction characters\n", 39), 0);
	return 1;
}

t_map *get_data(char *s, t_map *data)
{
	int fd = open(s, O_RDONLY);
	if (fd < 0)
		return (perror("open failed"), NULL);

	data = malloc(sizeof(t_map));
	if (!data)
		return (perror("malloc failed"), NULL);

	char **map_lines = get_map_lines(fd);
	if (!map_lines)
		return (printf("Failed to read map\n"), NULL);

	save_map_to_struct(data, map_lines);
	free_tab(map_lines);
	if(double_char(data) == 0 || is_map_surrounded(data) == 0)
			return (write(1, "Invalid map\n", 12), NULL);
	return(data);
}

// int main(int ac, char **av)
// {
// 	(void) ac;
// 	t_map *da;
// 	da = NULL;
// 	da = get_data(av[1], da);
// 	if(!da)
// 		return(write(1, "ERROR\n", 6), 1);
// 	free_tab(da->map);
// 	free(da);
// 	return 0;
// }

