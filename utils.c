/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/31 17:25:37 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/23 11:28:05 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*find_path(t_line **line)
{
	int		i;
	char	*buffer;
	char	*env_path;
	char	*cmd_path;
	char	**path;

	env_path = NULL;
	i = -1;
	while ((*line)->env[++i])
	{
		if (!ft_strncmp((*line)->env[i], "PATH=", 5))
		{
			env_path = ft_strdup((*line)->env[i] + 5);
			break ;
		}
	}
	if (access((*line)->cmds[0], F_OK | X_OK) == 0)
		return (ft_strdup((*line)->cmds[0]));
	path = ft_split(env_path, ':');
	i = -1;
	while (env_path && path[++i])
	{
		buffer = ft_strjoin(path[i], "/");
		cmd_path = ft_strjoin(buffer, (*line)->cmds[0]);
		free(buffer);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			free_charpp(path);
			return (cmd_path);
		}
		free(cmd_path);
	}
	if (path)
		error_display((*line)->cmds[0]);
	else
	{
		print_error("minishell: ");
        print_error((*line)->cmds[0]);
        print_error(": No such file or directory\n");
	}
	free_charpp(path);
	return (0);
}

char	**get_cmds(char *cmd)
{
	char	**cmds;
	char	**temp;
	char	**temp2;
	int		i;

	i = 0;
	if (ft_memchr(cmd, '/', 1))
	{
		temp = ft_split(cmd, ' ');
		if (access(temp[0], F_OK | X_OK) == 0)
		{
			temp2 = ft_split(cmd, '/');
			while (temp2[++i])
			cmds = ft_split(temp2[i], ' ');
		}
		else
		{
			perror(temp[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
		cmds = ft_split(cmd, ' ');
	return (cmds);
}

int	cmds_count(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}
