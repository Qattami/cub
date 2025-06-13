/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:19:21 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/13 01:37:35 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_map_line(char *line)
{
	while (*line == ' ' || *line == '\t')
		line++;
	return (*line == '1' || *line == '0');
}

int	compaire(char *line)
{
	return (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0
		|| ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0);
}

static int	process_line(char *line, char **map, int *i, int *map_started)
{
	if (!(*map_started))
	{
		if (line[0] == '\n' || ft_strlen(line) <= 1 || compaire(line))
			return (0);
		else if (is_map_line(line))
		{
			*map_started = 1;
			map[(*i)++] = ft_strdup(line);
			return (0);
		}
		return (0);
	}
	map[(*i)++] = ft_strdup(line);
	return (0);
}

char	**get_map_lines(int fd)
{
	char	**map;
	char	*line;
	int		i;
	int		map_started;

	map = malloc(sizeof(char *) * 1024);
	if (!map)
		return (NULL);
	i = 0;
	map_started = 0;
	line = get_next_line(fd);
	while (line)
	{
		process_line(line, map, &i, &map_started);
		free(line);
		line = get_next_line(fd);
	}
	map[i] = NULL;
	return (map);
}

t_map	*get_data(char *s, t_map *data)
{
	int		fd;
	char	**map_lines;

	fd = open(s, O_RDONLY);
	if (fd < 0)
		return (perror("open failed"), NULL);
	data = malloc(sizeof(t_map));
	if (!data)
		return (perror("malloc failed"), NULL);
	data->player_x = -1;
	data->player_y = -1;
	data->player_dir = '\0';
	map_lines = get_map_lines(fd);
	if (!map_lines)
		return (printf("Failed to read map\n"), NULL);
	save_map_to_struct(data, map_lines);
	free_tab(map_lines);
	if (double_char(data) == 0 || is_map_surrounded(data) == 0)
		return (free_map_data(data), write(1, "Invalid map\n", 12), NULL);
	return (data);
}
