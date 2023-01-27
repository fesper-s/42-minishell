/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 10:42:52 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/27 12:15:39 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//colocar no .h se for util
char	*smart_trim(char *cmd)
{
	char	*temp;

	temp = NULL;
	if (cmd[0] == '\'' || cmd[ft_strlen(cmd) - 1] == '\'')
		temp = ft_strtrim(cmd, "\'");
	else if (cmd[0] == '\"' || cmd[ft_strlen(cmd) - 1] == '\"')
		temp = ft_strtrim(cmd, "\"");
	else
		temp = ft_strdup(cmd);
	if (temp)
	{
		free(cmd);
		cmd = temp;
	}
	return (cmd);
}

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

int	check_dollar_sign(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '$')
			return (i);
	}
	return (-1);
}

int	check_cmdinenv(char *cmd, char **env)
{
	int		i;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], cmd, ft_strlen(cmd)) && env[i][ft_strlen(cmd)] == '=')
		{
			printf("%s", &env[i][ft_strlen(cmd) + 1]);
			return (count_cmdlen(env[i]));
		}
	}
	return (0);
}

int	check_expvar(char **cmds, t_env *env, int i)
{
	int		j;

	if (check_dollar_sign(cmds[i]) == -1)
		return (0);
	j = -1;
	while (cmds[i][++j])
	{
		if (cmds[i][j] != '$')
			printf("%c", cmds[i][j]);
		if (cmds[i][j] == '$' && !cmds[i][j + 1])
			printf("$");
		else if (cmds[i][j] == '$')
		{
			int	checking;
			checking = check_cmdinenv(&cmds[i][j + 1], env->env);
			if (checking)
				j += checking + 1;
			else
			{
				j++;
				while (cmds[i][j])
				{
					if (cmds[i][j] != '$')
						j++;
					else
					{
						j--;
						break ;
					}
				}
			}
		}
	}
	return (1);
}

int	handle_echo(char **cmds, t_env *env)
{
	int	isnull;
	int	newline;
	int	buffer;
	int	dollar_sign;
	int	i;

	g_status = 0;
	isnull = 0;
	if (cmds[1] == NULL)
		isnull = 1;
	i = 0;
	buffer = 0;
	while (!isnull && cmds[++i])
	{
		cmds[i] = smart_trim(cmds[i]);
		dollar_sign = check_expvar(cmds, env, i);
		newline = 1;
		check_newline(cmds, &newline, &buffer, i);
		if (!dollar_sign && cmds[i] && newline)
			printf("%s", cmds[i]);
		if (!dollar_sign && cmds[i] && cmds[i + 1] != NULL && newline)
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
