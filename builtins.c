/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 10:42:52 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/10 14:11:33 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pwd_and_env(char **cmds, char **env, char *pwd)
{
	int	i;

	if (!ft_strncmp(cmds[0], "pwd", 4))
	{
		i = 0;
		while (ft_strncmp(env[i], "PWD=", 4))
			i++;
		pwd = ft_strdup(env[i]);
		pwd = ft_strtrim(pwd, "PWD=");
		printf("%s\n", pwd);
	}
	if (!ft_strncmp(cmds[0], "env", 4))
	{
		i = -1;
		while (env[++i])
			printf("%s\n", env[i]);
	}
	g_status = 0;
	return (1);
}

int	handle_export(char **cmds, char **env)
{
	int	i;
	char **buffer;

	i = cmds_count(env);
	buffer = malloc(sizeof(char *) * (i + 2));
	i = -1;
	while (env[++i])
		buffer[i] = env[i];
	buffer[i] = cmds[1];
	buffer[i + 1] = 0; 
	free(env);
	i = -1;
	while (buffer[++i])
		env[i] = buffer[i];
	free(buffer);
	g_status = 0;
	return (1);
}

int	handle_builtins(char **cmds, char **env)
{
	int		i;
	char	*pwd;

	pwd = NULL;
	i = 0;
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
		return (handle_cd(cmds, env, pwd, i));
	if (!ft_strncmp(cmds[0], "pwd", 4) || !ft_strncmp(cmds[0], "env", 4))
		return (handle_pwd_and_env(cmds, env, pwd));
	if (!ft_strncmp(cmds[0], "export", 7))
		return (handle_export(cmds, env));
	return (0);
}
