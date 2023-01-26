/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 09:39:12 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/26 13:17:17 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	return_dir(t_env **env, int j)
{
	char	**bufferpp;
	char	*buffer;
	char	*pwd;

	bufferpp = ft_split((*env)->env[j], '/');
	buffer = ft_strtrim((*env)->env[j], \
		bufferpp[cmds_count(bufferpp) - 1]);
	free_charpp(bufferpp);
	pwd = ft_strtrim(buffer, "/");
	free(buffer);
	free((*env)->env[j]);
	(*env)->env[j] = ft_strdup(pwd);
	free(pwd);
}

void	relative_path(char *cmd, t_env **env, int j)
{
	char	*buffer;
	char	*pwd;

	if (!ft_strncmp(cmd, "..", 3))
		return_dir(env, j);
	else if (ft_strncmp(cmd, ".", 2))
	{
		buffer = ft_strdup((*env)->env[j]);
		pwd = ft_strjoin(buffer, "/");
		free(buffer);
		buffer = ft_strdup(pwd);
		free(pwd);
		pwd = ft_strjoin(buffer, cmd);
		free(buffer);
		free((*env)->env[j]);
		(*env)->env[j] = ft_strdup(pwd);
		free(pwd);
	}
}

void	chpwd(char *cmd, t_env **env, int j)
{
	char	*buffer;

	if (!ft_strncmp((*env)->env[j], "PWD=", 4))
	{
		if (ft_strncmp(cmd, "/", 2) && cmd[ft_strlen(cmd) - 1] == '/')
			cmd[ft_strlen(cmd) - 1] = 0;
		if (cmd[0] == '/')
		{
			buffer = ft_strjoin("PWD=", cmd);
			free((*env)->env[j]);
			(*env)->env[j] = ft_strdup(buffer);
			free(buffer);
		}
		else
			relative_path(cmd, env, j);
	}
}

char	*tilde_home(char *cmd, char *home)
{
	char	*buffer;
	char	*buffer_aux;

	buffer_aux = ft_strdup(home);
	buffer = ft_strjoin(buffer_aux, &cmd[1]);
	free(buffer_aux);
	free(cmd);
	cmd = ft_strdup(buffer);
	free(buffer);
	return (cmd);
}

int	handle_cd(char **cmds, t_env **env)
{
	char	*home;
	int		i;

	home = getenv("HOME");
	if (cmds[1] && cmds[1][0] == '~')
		cmds[1] = tilde_home(cmds[1], home);
	if (!cmds[1])
		chdir(home);
	else if (chdir(cmds[1]) < 0)
	{
		dir_error(cmds[1]);
		g_status = 1;
		return (2);
	}
	i = -1;
	while ((*env)->env[++i])
	{
		if (!cmds[1])
			chpwd(home, env, i);
		else
			chpwd(cmds[1], env, i);
	}
	g_status = 0;
	return (1);
}
