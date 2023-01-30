/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 10:42:52 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/30 09:06:15 by fesper-s         ###   ########.fr       */
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
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
			return (i);
	}
	return (-1);
}

int	check_cmdinenv(char *cmd, char **env)
{
	int		i;
	int		len;

	if (check_dollar_sign(cmd) != -1)
		len = check_dollar_sign(cmd);
	else
		len = ft_strlen(cmd);
	i = -1;

	while (env[++i])
	{
		if (!ft_strncmp(env[i], cmd, len) && env[i][len] == '=')
		{
			printf("%s", &env[i][len + 1]);
			return (count_cmdlen(env[i]));
		}
	}
	return (0);
}

// não ta no .h ainda
void	no_cmdinenv(char *cmd, int *i)
{
	*i += 1;
	while (cmd[*i])
	{
		if (ft_isalnum(cmd[*i]) || cmd[*i] == '_')
			*i += 1;
		else
		{
			*i -= 1;
			break ;
		}
	}
}

int	check_expvar(char *cmd, t_env *env)
{
	int		i;
	int		checking;

	if (check_dollar_sign(cmd) == -1)
		return (0);
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] != '$')
			printf("%c", cmd[i]);
		if (cmd[i] == '$' && !cmd[i + 1])
			printf("$");
		else if(cmd[i] == '$' && cmd[i + 1] == '?')
		{
			printf("%d", g_status);
			i++;
		}
		else if (cmd[i] == '$')
		{
			checking = check_cmdinenv(&cmd[i + 1], env->env);
			if (checking)
				i += checking;
			else
				no_cmdinenv(cmd, &i);
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

	isnull = 0;
	if (cmds[1] == NULL)
		isnull = 1;
	i = 0;
	buffer = 0;
	while (!isnull && cmds[++i])
	{
		cmds[i] = smart_trim(cmds[i]);
		dollar_sign = check_expvar(cmds[i], env);
		newline = 1;
		check_newline(cmds, &newline, &buffer, i);
		if (!dollar_sign && cmds[i] && newline)
			printf("%s", cmds[i]);
		if (cmds[i] && cmds[i + 1] != NULL && newline)
			printf(" ");
	}
	if (!buffer || isnull)
		printf("\n");
	g_status = 0;
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
