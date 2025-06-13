/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:22:16 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/13 02:12:57 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	textures(char **t, t_game ***game)
{
	int	i;

	i = 0;
	while (t[i])
	{
		if (!ft_strncmp(t[i], "NO ", 3))
			(**game)->textures.no_path = ft_strdup(t[i] + 3);
		else if (!ft_strncmp(t[i], "SO ", 3))
			(**game)->textures.so_path = ft_strdup(t[i] + 3);
		else if (!ft_strncmp(t[i], "EA ", 3))
			(**game)->textures.ea_path = ft_strdup(t[i] + 3);
		else if (!ft_strncmp(t[i], "WE ", 3))
			(**game)->textures.we_path = ft_strdup(t[i] + 3);
		else if (!ft_strncmp(t[i], "F ", 2) || !ft_strncmp(t[i], "C ", 2))
			;
		else
			return (1);
		i++;
	}
	return (0);
}

int	get_f_c(t_parse *d, t_game **game)
{
	char	**tab;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tab = get_6_lines();
	if (!tab || valid_trend(tab))
		return (write(2, "Failed to read lines\n", 21), 1);
	while (tab[i])
	{
		if (!ft_strncmp(tab[i], "C", 1))
			break ;
		i++;
	}
	while (tab[j])
	{
		if (!ft_strncmp(tab[j], "F", 1))
			break ;
		j++;
	}
	if (store_rgb(tab[i], tab[j], d) || textures(tab, &game))
		return (free_tab(tab), 1);
	return (free_tab(tab), 0);
}
