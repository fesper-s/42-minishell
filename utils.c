/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/31 17:25:37 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/02 10:11:32 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(char *cmd, char **envp)
{
	int		i;
	char	*temp;
	char	*env_path;
	char	*cmd_path;
	char	**path;

	i = -1;
	while (envp[++i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			env_path = ft_strnstr(envp[i], "PATH=", 5);
	}
	path = ft_split(env_path + 5, ':');
	i = -1;
	while (path[++i])
	{
		temp = ft_strjoin(path[i], "/");
		cmd_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(cmd_path, F_OK | X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
	}
	cmd_error(cmd);
	return (0);
}

char	**ft_trim(char **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		cmds[i] = ft_strtrim(cmds[i], "'\"");
		i++;
	}
	return (cmds);
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
	cmds = ft_trim(cmds);
	return (cmds);
}
