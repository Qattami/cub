/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:31:10 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/02 03:31:21 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	get_max_width_and_height(char **map_lines, int *height)
{
	int	i;
	int	len;
	int	max_width;

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
	*height = i;
	return (max_width);
}

static void	allocate_and_copy_map(t_map *data, char **map_lines)
{
	int	j;

	j = 0;
	while (j < data->map_h)
	{
		data->map[j] = malloc(data->map_w + 1);
		if (!data->map[j])
			return ;
		ft_memset(data->map[j], ' ', data->map_w);
		ft_memcpy(data->map[j], map_lines[j], ft_strlen(map_lines[j]));
		data->map[j][data->map_w] = '\0';
		j++;
	}
	data->map[data->map_h] = NULL;
}

void	save_map_to_struct(t_map *data, char **map_lines)
{
	data->map_w = get_max_width_and_height(map_lines, &data->map_h);
	data->map = malloc(sizeof(char *) * (data->map_h + 1));
	if (!data->map)
		return ;
	allocate_and_copy_map(data, map_lines);
}
