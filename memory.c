/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:28:11 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/30 12:31:03 by fesper-s         ###   ########.fr       */
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
