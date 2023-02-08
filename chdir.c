/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 09:39:12 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/07 13:33:50 by gussoare         ###   ########.fr       */
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

char	*no_argincd(char **env)
{
	char	*home;
	int		i;

	home = NULL;
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "HOME=", 5))
		{
			home = ft_strdup(env[i] + 5);
			break ;
		}
	}
	if (!home)
	{
		print_error("minishell: cd: HOME not set\n");
		g_status = 1;
	}
	else
		chdir(home);
	return (home);
}
