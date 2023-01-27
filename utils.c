/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/31 17:25:37 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/26 13:59:30 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **ft_strdupp(char **str)
{
	char	**temp;
	int		i;

	i = - 1;
	temp = malloc ((cmds_count(str) + 1) * sizeof(char *));
	while (str[++i])
		temp[i] = ft_strdup(str[i]);
	temp[i] = 0;
	return (temp);
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

char	*find_path(t_line **line)
{
	char	*cmd_path;
	char	*env_path;
	char	**path;

	env_path = check_for_path((*line)->env, NULL);
	if (access((*line)->cmds[0], F_OK | X_OK) == 0)
	{
		free(env_path);
		return (ft_strdup((*line)->cmds[0]));
	}
	path = ft_split(env_path, ':');
	cmd_path = check_cmdpath(env_path, path, (*line)->cmds[0]);
	free(env_path);
	if (cmd_path)
		return (cmd_path);
	path_error(path, (*line)->cmds[0]);
	free_charpp(path);
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
