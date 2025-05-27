/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 21:54:35 by iqattami          #+#    #+#             */
/*   Updated: 2023/11/16 05:18:11 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	count_word(char const *str, char charset)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] && str[i] == charset)
			i++;
		if (str[i])
			count++;
		while (str[i] && str[i] != charset)
			i++;
	}
	return (count);
}

static char	*malloc_word(char const *str, int i, char charset)
{
	int		j;
	char	*tab;
	int		len;

	len = 0;
	j = i;
	while (str[j] && str[j] != charset)
	{
		j++;
		len++;
	}
	tab = malloc(sizeof(char) * (len + 1));
	if (!tab)
		return (NULL);
	j = 0;
	while (j < len)
	{
		tab[j] = str[i];
		i++;
		j++;
	}
	tab[j] = '\0';
	return (tab);
}

static char	**free_split(char **str, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

static char	**sub_split(char const *str, char charset, char **tab)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && str[i] == charset)
			i++;
		if (str[i])
		{
			tab[j] = malloc_word(str, i, charset);
			if (tab[j] == NULL)
				return (free_split(tab, j));
			j++;
		}
		while (str[i] && str[i] != charset)
			i++;
	}
	tab[j] = 0;
	return (tab);
}

char	**ft_split(char const *str, char charset)
{
	char	**tab;

	if (str == NULL)
		return (NULL);
	tab = (char **)malloc(sizeof(char *) * (count_word(str, charset) + 1));
	if (!tab)
		return (NULL);
	tab = sub_split(str, charset, tab);
	return (tab);
}
