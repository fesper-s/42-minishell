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

int	tilpipe(char **cmds)
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

int	count_export_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

int	check_dir(char **cmds)
{
	DIR	*dir;

	if (!cmds[0])
		return (0);
	dir = opendir(cmds[0]);
	if (dir)
	{
		check_dir_error(cmds[0]);
		closedir(dir);
		return (1);
	}
	return (0);
}

int	is_builtin(t_line **line)
{
	if (!ft_strncmp((*line)->cmds[0], "export", 7))
		return (1);
	if (!ft_strncmp((*line)->cmds[0], "unset", 6))
		return (1);
	if (!ft_strncmp((*line)->cmds[0], "env", 4))
		return (1);
	if (!ft_strncmp((*line)->cmds[0], "cd", 3))
		return (1);
	if (!ft_strncmp((*line)->cmds[0], "pwd", 4))
		return (1);
	if (!ft_strncmp((*line)->cmds[0], "echo", 5))
		return (1);
	return (0);
}
