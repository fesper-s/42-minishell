/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 08:33:43 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/16 09:48:05 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_wordcount(char const *s, char c)
{
	size_t	i;
	size_t	wordnbr;

	i = 0;
	wordnbr = 0;
	while (s[i])
	{
		if (!(s[i] == c))
		{
			wordnbr++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (wordnbr);
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
