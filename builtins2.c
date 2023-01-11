/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 09:39:12 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/11 08:22:06 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	relative_path(char *cmd, t_env **env, char *pwd, int j)
{
	char	**buffer;
	int		i;

	if (!ft_strncmp(cmd, "..", 3))
	{
		buffer = ft_split((*env)->env[j], '/');
		i = 0;
		while (buffer[i])
			i++;
		pwd = ft_strtrim((*env)->env[j], buffer[i - 1]);
		pwd = ft_strtrim(pwd, "/");
		(*env)->env[j] = ft_strdup(pwd);
	}
	else if (ft_strncmp(cmd, ".", 2))
	{
		pwd = ft_strdup((*env)->env[j]);
		pwd = ft_strjoin(pwd, "/");
		pwd = ft_strjoin(pwd, cmd);
		(*env)->env[j] = ft_strdup(pwd);
	}
}

void	chpwd(char *cmd, t_env **env, int j)
{
	int		i;
	char	*buffer;

	buffer = NULL;
	if (!ft_strncmp((*env)->env[j], "PWD=", 4))
	{
		i = 0;
		while (cmd[i])
			i++;
		if (cmd[i - 1] == '/')
			cmd[i - 1] = 0;
		if (cmd[0] == '/')
		{
			buffer = "PWD=";
			buffer = ft_strjoin(buffer, cmd);
			(*env)->env[j] = ft_strdup(buffer);
		}
		else
			relative_path(cmd, env, buffer, j);
		free(buffer);
	}
}

int	handle_cd(char **cmds, t_env **env)
{
	int	i;

	if (chdir(cmds[1]) < 0)
	{
		dir_error(cmds[1]);
		g_status = 1;
		return (2);
	}
	i = -1;
	while ((*env)->env[++i])
		chpwd(cmds[1], env, i);
	g_status = 0;
	return (1);
}
