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

int	count_export_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

int	check_dir(char **cmds, char **env)
{
	int	i;

	if (!cmds[0])
		return (0);
	i = -1;
	while (env[++i])
		if (!ft_strncmp(env[i], "PWD=", 4))
			break ;
	if (!chdir(cmds[0]))
	{
		chdir(env[i]);
		check_dir_error(cmds[0]);
		return (1);
	}
	return (0);
}

int	is_builtin(t_line **line)
{
	int	i;

	i = 0;
	if (!ft_strncmp((*line)->cmds[0], "export", 7))
		i = 1;
	if (!ft_strncmp((*line)->cmds[0], "unset", 6))
		i = 1;
	if (!ft_strncmp((*line)->cmds[0], "env", 4))
		i = 1;
	if (!ft_strncmp((*line)->cmds[0], "cd", 3))
		i = 1;
	if (!ft_strncmp((*line)->cmds[0], "pwd", 4))
		i = 1;
	if (!ft_strncmp((*line)->cmds[0], "echo", 5))
		i = 1;
	if (i == 1)
		g_status = 0;
	return (0);
}
