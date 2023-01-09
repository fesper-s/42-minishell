/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 08:33:43 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/06 12:45:51 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wordcount(char const *s, char c)
{
	size_t	i;
	size_t	j;

	i = 1;
	j = -1;
	while (*s && *s == c)
		s++;
	while (s[++j])
	{
		if (s[j] != c && s[j - 1] == c)
			i++;
		if (s[j] == '"')
		{
			j++;
			while (s[j] != '"')
				j++;
		}
		if (s[j] == 39)
		{
			j++;
			while (s[j] != 39)
				j++;
		}
	}
	return (i);
}

static size_t	ft_strsize(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] != 0 && s[i] != c)
	{
		if (s[i] == '"')
		{
			i++;
			while (s[i] != '"')
				i++;
		}
		if (s[i] == 39)
		{
			i++;
			while (s[i] != 39)
				i++;
		}
		i++;
	}
	return (i);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	char	**str;

	i = 0;
	j = 0;
	if (!s)
		return (0);
	str = malloc((ft_wordcount(s, c) + 1) * sizeof(char *));
	if (str == 0)
		return (0);
	while (s[j] != 0 && i < ft_wordcount(s, c))
	{
		while (s[j] == c)
			j++;
		str[i] = ft_substr(&s[j], 0, ft_strsize(&s[j], c));
		j += ft_strsize(&s[j], c);
		i++;
	}
	str[i] = 0;
	return (str);
}
