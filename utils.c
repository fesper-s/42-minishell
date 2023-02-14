/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/31 17:25:37 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/14 12:20:02 by fesper-s         ###   ########.fr       */
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

void	validate_dir(char **cmds)
{
	DIR	*dir;
	int	i;
	int	fd;

	g_status = 0;
	i = 0;
	while (cmds[++i])
	{
		dir = opendir(cmds[i]);
		fd = open(cmds[i], O_RDONLY);
		if (!dir && cmds[i] && cmds[i][0] != '-' && fd == -1)
			g_status = 1;
		close(fd);
		if (dir)
			closedir(dir);
	}
}

char	*find_path(t_line **line, t_env **env)
{
	char	*cmd_path;
	char	*env_path;
	char	**path;

	if (!(*line)->cmds[0] || is_builtin(line))
	{
		if (!(*line)->cmds[0])
			g_status = 127;
		return (NULL);
	}
	validate_dir((*line)->cmds);
	env_path = check_for_path((*env)->env, NULL);
	if (access((*line)->cmds[0], F_OK | X_OK) == 0)
	{
		g_status = 126;
		if (!ft_strncmp((*line)->cmds[0], "/bin", 4) || \
			!ft_strncmp((*line)->cmds[0], "/usr/bin", 8) || \
			!ft_strncmp((*line)->cmds[0], "./", 2))
			g_status = 0;
		free(env_path);
		return (ft_strdup((*line)->cmds[0]));
	}
	path = ft_split(env_path, ':');
	cmd_path = check_cmdpath(env_path, path, (*line)->cmds[0]);
	if (cmd_path)
		return (cmd_path);
	path_error(path, (*line)->cmds[0]);
	free_ppp(env_path, path);
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
