/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/31 17:25:37 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/19 11:15:01 by gussoare         ###   ########.fr       */
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

char	*find_path(char *cmd)
{
	int		i;
	char	*buffer;
	char	*env_path;
	char	*cmd_path;
	char	**path;

	env_path = getenv("PATH");
	path = ft_split(env_path, ':');
	i = -1;
	while (path[++i])
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
	error_display(cmd);
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
