/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 02:51:33 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/13 01:42:03 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_valid_tile(char c)
{
	return (c == '0' || ft_strchr("NSEW", c));
}

int	is_valid_char(char c)
{
	return (c == '0' || c == '1' || c == ' ' || ft_strchr("NSEW", c));
}

int	compaire_data(t_map *d, int i, int j)
{
	return (d->map[i - 1][j] == ' ' || d->map[i + 1][j] == ' ' || d->map[i][j
		- 1] == ' ' || d->map[i][j + 1] == ' ');
}

int	is_map_surrounded(t_map *d)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < d->map_h)
	{
		j = 0;
		while (j < d->map_w)
		{
			c = d->map[i][j];
			if (!is_valid_char(c))
				return (write(1, "Invalid character\n", 18), 0);
			if (is_valid_tile(c))
			{
				if (i == 0 || j == 0 || i == d->map_h - 1 || j == d->map_w - 1)
					return (write(1, "Open edge\n", 10), 0);
				if (compaire_data(d, i, j))
					return (write(1, "Open space\n", 11), 0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	double_char(t_map *d)
{
	init_var(d);
	while (d->i < d->map_h)
	{
		d->j = 0;
		while (d->j < d->map_w)
		{
			d->c = d->map[d->i][d->j];
			if (d->c == 'N' || d->c == 'S' || d->c == 'E' || d->c == 'W')
			{
				if (d->count == 0)
				{
					d->player_x = d->j;
					d->player_y = d->i;
					d->player_dir = d->c;
					d->map[d->i][d->j] = '0';
				}
				d->count++;
			}
			d->j++;
		}
		d->i++;
	}
	if (d->count != 1)
		return (write(1, "Invalid number of direction characters\n", 39), 0);
	return (1);
}
