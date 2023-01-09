/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 10:42:52 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/09 15:13:29 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtins(char **cmds, char **envp)
{
	int		i;
	int		j;
	char	*pwd;
	char	**buffer;

	if (!ft_strncmp(cmds[0], "echo", 5))
	{
		i = 0;
		while (cmds[++i])
		{
			if (ft_strncmp(cmds[i], "-n", 3))
				printf("%s", cmds[i]);
			if (cmds[i + 1] != NULL && ft_strncmp(cmds[i], "-n", 3))
				printf(" ");
		}
		if (ft_strncmp(cmds[1], "-n", 3))
			printf("\n");
		g_status = 0;
		return (1);
	}
	if (!ft_strncmp(cmds[0], "cd", 3))
	{
		if (chdir(cmds[1]) < 0)
		{
			dir_error(cmds[1]);
			g_status = 1;
			return (2);
		}
		i = -1;
		while (envp[++i])
		{
			if (!ft_strncmp(envp[i], "PWD=", 4))
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
					envp[i] = ft_strdup(pwd);
				}
				else
				{
					if (!ft_strncmp(cmds[1], "..", 3))
					{
						buffer = ft_split(envp[i], '/');
						j = 0;
						while (buffer[j])
							j++;
						pwd = ft_strtrim(envp[i], buffer[j - 1]);
						pwd = ft_strtrim(pwd, "/");
						envp[i] = ft_strdup(pwd);
					}
					else
					{
						pwd = ft_strdup(envp[i]);
						pwd = ft_strjoin(pwd, "/");
						pwd = ft_strjoin(pwd, cmds[1]);
						envp[i] = ft_strdup(pwd);
					}
				}
				free(pwd);
			}
		}
		g_status = 0;
		return (1);
	}
	if (!ft_strncmp(cmds[0], "pwd", 4))
	{
		i = 0;
		while (ft_strncmp(envp[i], "PWD=", 4))
			i++;
		pwd = ft_strdup(envp[i]);
		pwd = ft_strtrim(pwd, "PWD=");
		printf("%s\n", pwd);
		g_status = 0;
		return (1);
	}
	if (!ft_strncmp(cmds[0], "env", 4))
	{
		i = -1;
		while (envp[++i])
			printf("%s\n", envp[i]);
		g_status = 0;
		return (1);
	}
	return (0);
}
