/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:33:25 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/13 02:11:37 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_tab(char **t)
{
	int	i;

	i = 0;
	if (t != NULL)
	{
		while (t[i])
		{
			free(t[i]);
			i++;
		}
		free(t);
	}
}

int	valid_digit(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		return (0);
	}
	while (str[i])
	{
		if (ft_isdigit(str[i]))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

char	**colors_rgb(char *line)
{
	char	**table;
	int		i;

	table = ft_split(line + 2, ',');
	i = 0;
	while (table[i])
	{
		if (valid_digit(table[i]))
			return (free_tab(table), write(2, "Invalide rgb\n", 13), NULL);
		i++;
	}
	return (table);
}

int	v_rgb(int i)
{
	if (i < 0 || i > 255)
		return (1);
	return (0);
}

int	store_rgb(char *c, char *s, t_parse *d)
{
	char	**t;
	char	**l;

	d->f_c = s[0];
	d->next->f_c = c[0];
	t = colors_rgb(s);
	l = colors_rgb(c);
	if (!t || !l)
		return (1);
	d->r = ft_atoi(t[0]);
	d->g = ft_atoi(t[1]);
	d->b = ft_atoi(t[2]);
	d->next->r = ft_atoi(l[0]);
	d->next->g = ft_atoi(l[1]);
	d->next->b = ft_atoi(l[2]);
	if (v_rgb(d->r) == 1 || v_rgb(d->g) == 1 || v_rgb(d->b) == 1
		|| v_rgb(d->next->r) == 1 || v_rgb(d->next->g) == 1
		|| v_rgb(d->next->b) == 1)
		return (free_tab(t), free_tab(l), write(2, "Invalide rgb here\n", 13),
			1);
	return (free_tab(t), free_tab(l), 0);
}
