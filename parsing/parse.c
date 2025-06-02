/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 03:38:37 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/02 03:44:33 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	pars_extension(char *s, char *s2)
{
	int	len;
	int	i;
	int	j;

	j = 3;
	len = ft_strlen(s);
	len--;
	i = len - 3;
	while (len >= i)
	{
		if (s[len] != s2[j])
		{
			return (1);
		}
		else
		{
			j--;
			len--;
		}
	}
	return (0);
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

int	existe(int *t)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		if (t[i] != 1)
			return (1);
		i++;
	}
	return (0);
}

int	valid_trend(char **t)
{
	int	i;
	int	tl[5];

	ft_bzero(tl, 5);
	i = 0;
	while (i < 6)
	{
		if (t[i][0] == 'S' && t[i][1] == 'O')
			tl[0] = 1;
		else if (t[i][0] == 'N' && t[i][1] == 'O')
			tl[1] = 1;
		else if (t[i][0] == 'W' && t[i][1] == 'E')
			tl[2] = 1;
		else if (t[i][0] == 'E' && t[i][1] == 'A')
			tl[3] = 1;
		else if (i < 5 && ((!ft_strncmp(t[i], "C ", 2) && !ft_strncmp(t[i + 1],
						"F ", 2)) || (!ft_strncmp(t[i], "F ", 2)
					&& !ft_strncmp(t[i + 1], "C ", 2))))
			tl[4] = 1;
		i++;
	}
	return (existe(tl));
}

t_parse	*get_pars(t_parse *pars, t_game *game)
{
	pars = malloc(sizeof(t_parse));
	if (!pars)
		return (NULL);
	pars->next = malloc(sizeof(t_parse));
	if (!pars->next)
	{
		free(pars);
		return (NULL);
	}
	if (!get_f_c(pars, game))
		printf("RGB stored: R=%d G=%d B=%d\n", pars->r, pars->g, pars->b);
	else
		return (free(pars->next), free(pars), NULL);
	printf("RGB stored: R=%d G=%d B=%d\n", pars->next->r, pars->next->g,
		pars->next->b);
	return (pars);
}
