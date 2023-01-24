/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 10:42:52 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/23 13:21:38 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pwd(t_env *env)
{
	int		i;
	char	*buffer;

	i = 0;
	while (ft_strncmp(env->env[i], "PWD=", 4))
		i++;
	buffer = ft_strdup(env->env[i]);
	buffer = ft_strtrim(buffer, "PWD=");
	printf("%s\n", buffer);
	free(buffer);
	g_status = 0;
	return (1);
}

int	handle_env(t_env *env)
{
	int		i;
	int		path;

	path = 0;
	i = -1;
	while (env->env[++i])
		if (!ft_strncmp(env->env[i], "PATH=", 5))
			path = 1;
	i = -1;
	while (path && env->env[++i])
		printf("%s\n", env->env[i]);
	if (!path)
		path_error(NULL, "env");
	g_status = 0;
	return (1);
}

void	check_expvar(char **cmds, t_env *env, int j)
{
	int		i;
	char	*buffer;

	if (cmds[j][0] == '$')
	{
		i = -1;
		while (env->env[++i])
		{
			if (!ft_strncmp(&cmds[j][1], env->env[i], ft_strlen(&cmds[j][1])) \
				&& env->env[i][ft_strlen(&cmds[j][1])] == '=')
			{
				buffer = ft_strdup(&env->env[i][count_cmdlen(env->env[i]) + 1]);
				free(cmds[j]);
				cmds[j] = ft_strdup(buffer);
				free(buffer);
				break ;
			}
			if (env->env[i + 1] == NULL)
			{
				free(cmds[j]);
				cmds[j] = malloc(sizeof(char) * 1);
				cmds[j] = 0;
			}
		}
	}
}

int	handle_echo(char **cmds, t_env *env)
{
	int	isnull;
	int	newline;
	int	buffer;
	int	i;

	g_status = 0;
	isnull = 0;
	if (cmds[1] == NULL)
		isnull = 1;
	i = 0;
	buffer = 0;
	while (!isnull && cmds[++i])
	{
		check_expvar(cmds, env, i);
		newline = 1;
		check_newline(cmds, &newline, &buffer, i);
		if (cmds[i] && newline)
			printf("%s", cmds[i]);
		if (cmds[i] && cmds[i + 1] != NULL && newline)
			printf(" ");
	}
	if (!buffer || isnull)
		printf("\n");
	return (1);
}

int	handle_builtins(char **cmds, t_env **env)
{
	if (!ft_strncmp(cmds[0], "echo", 5))
		return (handle_echo(cmds, *env));
	if (!ft_strncmp(cmds[0], "cd", 3))
		return (handle_cd(cmds, env));
	if (!ft_strncmp(cmds[0], "pwd", 4))
		return (handle_pwd(*env));
	if (!ft_strncmp(cmds[0], "env", 4))
		return (handle_env(*env));
	if (!ft_strncmp(cmds[0], "export", 7))
		return (handle_export(cmds, env));
	if (!ft_strncmp(cmds[0], "unset", 6))
		return (handle_unset(cmds, env));
	return (0);
}
