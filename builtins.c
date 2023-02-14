/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 10:42:52 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/14 10:00:22 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	smart_trim(t_line **line, int index)
{
	char	*temp;

	temp = NULL;
	if ((*line)->cmds[index][0] == '\'' || (*line)->cmds[index][\
		ft_strlen((*line)->cmds[index]) - 1] == '\'')
		temp = ft_strtrim((*line)->cmds[index], "\'");
	else if ((*line)->cmds[index][0] == '\"' || (*line)->cmds[index][\
		ft_strlen((*line)->cmds[index]) - 1] == '\"')
		temp = ft_strtrim((*line)->cmds[index], "\"");
	else
		temp = ft_strdup((*line)->cmds[index]);
	if (temp)
	{
		free((*line)->cmds[index]);
		(*line)->cmds[index] = ft_strdup(temp);
		free(temp);
	}
	if (!ft_strlen((*line)->cmds[0]))
	{
		free((*line)->cmds[index]);
		(*line)->cmds[index] = malloc(sizeof(char *));
		(*line)->cmds[index] = 0;
	}
}

int	handle_cd(char **cmds, t_env **env)
{
	char	*home;
	int		i;

	if (cmds[1] && cmds[1][0] == '~')
		cmds[1] = tilde_home(cmds[1], getenv("HOME"));
	if (!cmds[1])
		home = no_argincd((*env)->env);
	else if (chdir(cmds[1]) < 0)
	{
		dir_error(cmds[1]);
		g_status = 1;
		return (2);
	}
	i = -1;
	while ((*env)->env[++i])
	{
		if (!cmds[1] && home)
			chpwd(home, env, i);
		else if (cmds[1])
			chpwd(cmds[1], env, i);
	}
	if (!cmds[1] && home)
		free(home);
	g_status = 0;
	return (1);
}

int	handle_pwd(t_env *env)
{
	int		i;
	char	*buffer;
	char	*buffer_aux;

	i = 0;
	while (ft_strncmp(env->env[i], "PWD=", 4))
		i++;
	buffer_aux = ft_strdup(env->env[i]);
	buffer = ft_strtrim(buffer_aux, "PWD=");
	printf("%s\n", buffer);
	free(buffer);
	free(buffer_aux);
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

int	handle_builtins(char **cmds, t_env **env)
{
	if (!cmds[0])
		return (0);
	if (!ft_strncmp(cmds[0], "echo", 5))
		return (handle_echo(cmds));
	if (!ft_strncmp(cmds[0], "pwd", 4))
		return (handle_pwd(*env));
	if (!ft_strncmp(cmds[0], "env", 4))
		return (handle_env(*env));
	return (0);
}
