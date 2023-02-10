/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/31 17:25:37 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/08 08:49:42 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_for_path(char **env, char *env_path)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
		{
			env_path = ft_strdup(env[i] + 5);
			return (env_path);
		}
	}
	return (NULL);
}

char	*check_cmdpath(char *env_path, char **path, char *cmd)
{
	char	*buffer;
	char	*cmd_path;
	int		i;

	i = -1;
	while (env_path && path[++i])
	{
		buffer = ft_strjoin(path[i], "/");
		cmd_path = ft_strjoin(buffer, cmd);
		free(buffer);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_charpp(path);
			return (cmd_path);
		}
		free(cmd_path);
	}
	return (NULL);
}

int	is_builtin(t_line **line)
{
	int i;

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

char	*find_path(t_line **line, t_env **env)
{
	char	*cmd_path;
	char	*env_path;
	char	**path;

	g_status = 127;
	if (!(*line)->cmds[0] || is_builtin(line))
		return (NULL);
	env_path = check_for_path((*env)->env, NULL);
	if (access((*line)->cmds[0], F_OK | X_OK) == 0)
	{
		free(env_path);
		g_status = 0;
		return (ft_strdup((*line)->cmds[0]));
	}
	path = ft_split(env_path, ':');
	cmd_path = check_cmdpath(env_path, path, (*line)->cmds[0]);
	free(env_path);
	g_status = 0;
	if (cmd_path)
		return (cmd_path);
	path_error(path, (*line)->cmds[0]);
	free_charpp(path);
	g_status = 127;
	return (0);
}

int	cmds_count(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}
