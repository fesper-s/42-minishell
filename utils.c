/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/31 17:25:37 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/14 09:07:33 by gussoare         ###   ########.fr       */
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

char	*find_path(t_line **line, t_env **env)
{
	char	*cmd_path;
	char	*env_path;
	char	**path;
	DIR		*dir;
	int		i;

	if (!(*line)->cmds[0] || is_builtin(line))
		return (NULL);
	env_path = check_for_path((*env)->env, NULL);
	if (access((*line)->cmds[0], F_OK | X_OK) == 0)
	{
		g_status = 126;
		free(env_path);
		return (ft_strdup((*line)->cmds[0]));
	}
	path = ft_split(env_path, ':');
	cmd_path = check_cmdpath(env_path, path, (*line)->cmds[0]);
	free(env_path);
	g_status = 0;
	i = 0;
	while ((*line)->cmds[++i])
	{
		dir = opendir((*line)->cmds[i]);
		int fd;
		fd = open((*line)->cmds[i], O_RDONLY);
		if (!dir && (*line)->cmds[i] && (*line)->cmds[i][0] != '-' && fd == -1)
			g_status = 1;
		close(fd);
		if (dir)
			closedir(dir);
	}
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

void	free_lstcontent(t_line **buffer)
{
	if ((*buffer)->insert_char)
		free_charpp((*buffer)->insert_char);
	if ((*buffer)->insert_op)
		free((*buffer)->insert_op);
	if ((*buffer)->path)
		free((*buffer)->path);
	if ((*buffer)->cmd)
		free((*buffer)->cmd);
	if ((*buffer)->infile)
		free((*buffer)->infile);
	if ((*buffer)->outfile)
		free((*buffer)->outfile);
}
