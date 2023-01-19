/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 13:04:16 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/17 13:04:16 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_flag(char **cmds, int i)
{
	int	j;

	while (--i >= 1)
	{
		j = 0;
		if (cmds[i][j] == '-')
			j++;
		while (cmds[i][j])
		{
			if (cmds[i][j] != 'n')
				return (0);
			j++;
		}
	}
	return (1);
}

void	check_newline(char **cmds, int *newline, int *buffer, int i)
{
	int	j;

	if (cmds[i] && !ft_strncmp(cmds[i], "-n", 2))
	{
		*newline = 1;
		j = 1;
		while (cmds[i][j])
		{
			if (cmds[i][j] != 'n')
				break ;
			j++;
			if (!cmds[i][j] && is_flag(cmds, i))
			{
				*newline = 0;
				*buffer = 1;
			}
		}
	}
}

int	cmds_til_pipe(char **cmds)
{
	int	i;

	i = -1;
	while (cmds[++i])
	{
		if (cmds[i][0] == '|')
			break ;
	}
	return (i);
}
