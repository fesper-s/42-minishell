/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 10:42:52 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/18 14:24:14 by fesper-s         ###   ########.fr       */
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

void	handle_echo(char **cmds)
{
	int	newline;
	int	buffer;
	int	i;

	i = 0;
	buffer = 0;
	while (cmds[++i])
	{
		newline = 1;
		check_newline(cmds, &newline, &buffer, i);
		if (newline)
			printf("%s", cmds[i]);
		if (cmds[i + 1] != NULL && newline)
			printf(" ");
	}
	if (!buffer)
		printf("\n");
}

int	handle_builtins(char **cmds, t_env **env)
{
	if (!ft_strncmp(cmds[0], "echo", 5))
	{
		g_status = 0;
		if (cmds[1] == NULL)
		{
			printf("\n");
			return (1);
		}
		handle_echo(cmds);
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
