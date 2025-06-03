/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_lines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:42:18 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/03 16:08:58 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	is_config_line(char *line, int *k)
{
	*k = 0;
	while (line[*k] == ' ' || line[*k] == '\t')
		(*k)++;
	if (ft_strncmp(&line[*k], "NO ", 3) == 0 || ft_strncmp(&line[*k], "SO ",
			3) == 0 || ft_strncmp(&line[*k], "WE ", 3) == 0
		|| ft_strncmp(&line[*k], "EA ", 3) == 0 || ft_strncmp(&line[*k], "F ",
			2) == 0 || ft_strncmp(&line[*k], "C ", 2) == 0)
		return (1);
	return (0);
}

int	only_spaces_or_tabs(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

static void	read_config_lines(int fd, char **t)
{
	char	*line;
	int		k;
	int		i;

	i = 0;
	line = get_next_line(fd);
	while (line && i < 6)
	{
		if (only_spaces_or_tabs(line))
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if (is_config_line(line, &k))
			t[i++] = helper31(line, k);
		else if (end_of_read_lines(line, t))
			return ;
		free(line);
		if (i < 6)
			line = get_next_line(fd);
	}
	t[i] = NULL;
}

char	**get_6_lines(void)
{
	char	**t;
	int		fd;

	t = malloc(sizeof(char *) * 7);
	if (!t)
		return (NULL);
	fd = open("../maps/map.cub", O_RDONLY);
	if (fd < 0)
		return (NULL);
	read_config_lines(fd, t);
	close(fd);
	if (!t[5])
		return (free_tab(t), NULL);
	return (t);
}
int	main(int ac, char **av)
{
	t_parse	*pars;
	t_game	game;
	t_map	*da;

	(void)ac;
	pars = NULL;
	da = NULL;
	pars = get_pars(pars, &game);
	if (!pars)
		return (write(1, "ERROR\n", 6), 1);
	da = get_data(av[1], da);
	if (!da)
		return (write(1, "ERROR1\n", 7), 1);
	init_game(&game, da, pars);
	mlx_hook(game.win, 2, 1L << 0, key_press, &game.player);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game.player);
	mlx_loop_hook(game.mlx, draw_loop, &game);
	mlx_loop(game.mlx);
	// free_tab(da->map);
	// free(da);
	// free(pars);
	// free(pars->next);
	// free_map_data(da);
	// mlx_destroy_window(game.mlx, game.win);
	// free_textures(&game.textures, &game);
	cleanup_game(&game, da, pars);
	return (0);
}
