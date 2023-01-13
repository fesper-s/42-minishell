/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 10:42:52 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/13 12:08:16 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pwd_and_env(char **cmds, t_env *env)
{
	int		i;
	char	*buffer;

	if (!ft_strncmp(cmds[0], "pwd", 4))
	{
		i = 0;
		while (ft_strncmp(env->env[i], "PWD=", 4))
			i++;
		buffer = ft_strdup(env->env[i]);
		buffer = ft_strtrim(buffer, "PWD=");
		printf("%s\n", buffer);
		free(buffer);
	}
	if (!ft_strncmp(cmds[0], "env", 4))
	{
		i = -1;
		while (env->env[++i])
			printf("%s\n", env->env[i]);
	}
	g_status = 0;
	return (1);
}

int	handle_export(char **cmds, t_env **env)
{
	int		i;
	char	**buffer;

	if (ft_isalpha(cmds[1][0]) || cmds[1][0] == '_')
	{
		i = 0;
		while (cmds[1][++i])
		{
			if (cmds[1][i] == '=')
				break ;
			if (!ft_isalnum(cmds[1][i]))
			{
				export_error(cmds[1]);
				return (1);
			}
		}
		i = cmds_count((*env)->env);
		buffer = malloc(sizeof(char *) * (i + 2));
		i = -1;
		while ((*env)->env[++i])
			buffer[i] = (*env)->env[i];
		buffer[i] = ft_strdup(cmds[1]);
		buffer[i + 1] = 0;
		i = -1;
		while (buffer[++i])
			(*env)->env[i] = buffer[i];
		(*env)->env[i] = 0;
		g_status = 0;
	}
	else
		export_error(cmds[1]);
	return (1);
}

int	handle_builtins(char **cmds, t_env **env)
{
	int		i;

	if (!ft_strncmp(cmds[0], "echo", 5))
	{
		g_status = 0;
		if (cmds[1] == NULL)
		{
			printf("\n");
			return (1);
		}
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
		return (1);
	}
	if (!ft_strncmp(cmds[0], "cd", 3))
		return (handle_cd(cmds, env));
	if (!ft_strncmp(cmds[0], "pwd", 4) || !ft_strncmp(cmds[0], "env", 4))
		return (handle_pwd_and_env(cmds, *env));
	if (!ft_strncmp(cmds[0], "export", 7))
		return (handle_export(cmds, env));
	return (0);
}
