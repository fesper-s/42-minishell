/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:28:11 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/14 08:57:54 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_strdupp(char **str)
{
	char	**buffer;
	int		i;

	i = -1;
	buffer = malloc((cmds_count(str) + 1) * sizeof(char *));
	while (str[++i])
		buffer[i] = ft_strdup(str[i]);
	buffer[i] = 0;
	return (buffer);
}

int	free_charpp(char **str)
{
	int	i;

	i = -1;
	if (!str)
		return (0);
	while (str[++i])
		free(str[i]);
	free(str);
	return (0);
}

void	exiting(t_line **line, t_env **env)
{
	free((*line)->cmd);
	free_charpp((*line)->cmds);
	if (*line)
		free(*line);
	free_charpp((*env)->env);
	free(*env);
	printf("exit\n");
}

void	free_two(char **p1, char **p2)
{
	free(p1[0]);
	free(p2[0]);
}

void	free_ppp(char *p, char **pp)
{
	free(p);
	free_charpp(pp);
}
