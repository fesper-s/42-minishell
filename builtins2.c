/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 09:39:12 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/10 14:22:04 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	relative_path(char **cmds, char **env, char *pwd, int i)
{
	char	**buffer;
	int		j;

	if (!ft_strncmp(cmds[1], "..", 3))
	{
		buffer = ft_split(env[i], '/');
		j = 0;
		while (buffer[j])
			j++;
		pwd = ft_strtrim(env[i], buffer[j - 1]);
		pwd = ft_strtrim(pwd, "/");
		env[i] = ft_strdup(pwd);
	}
	else if (ft_strncmp(cmds[1], ".", 2))
	{
		pwd = ft_strdup(env[i]);
		pwd = ft_strjoin(pwd, "/");
		pwd = ft_strjoin(pwd, cmds[1]);
		env[i] = ft_strdup(pwd);
	}
}

void	chpwd(char **cmds, char **env, char *pwd, int i)
{
	int	j;

	if (!ft_strncmp(env[i], "PWD=", 4))
	{
		j = 0;
		while (cmds[1][j])
			j++;
		if (cmds[1][j - 1] == '/')
			cmds[1][j - 1] = 0;
		if (cmds[1][0] == '/')
		{
			pwd = "PWD=";
			pwd = ft_strjoin(pwd, cmds[1]);
			env[i] = ft_strdup(pwd);
		}
		else
			relative_path(cmds, env, pwd, i);
		free(pwd);
	}
}

int	handle_cd(char **cmds, char **env, char *pwd, int i)
{
	if (chdir(cmds[1]) < 0)
	{
		dir_error(cmds[1]);
		g_status = 1;
		return (2);
	}
	i = -1;
	while (env[++i])
		chpwd(cmds, env, pwd, i);
	g_status = 0;
	return (1);
}
