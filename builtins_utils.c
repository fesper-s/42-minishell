/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 13:04:16 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/17 13:04:16 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_flag(char **cmds, int i)
{
	int	j;

	while (--i >= 1)
	{
		j = 0;
		if (cmds[i][j] == '-')
			j++;
		while (cmds[i][j])
		{
			if (cmds[i][j] != 'n')
				return (0);
			j++;
		}
	}
	return (1);
}

void	check_newline(char **cmds, int *newline, int *buffer, int i)
{
	int	j;

	if (cmds[i] && !ft_strncmp(cmds[i], "-n", 2))
	{
		*newline = 1;
		j = 1;
		while (cmds[i][j])
		{
			if (cmds[i][j] != 'n')
				break ;
			j++;
			if (!cmds[i][j] && is_flag(cmds, i))
			{
				*newline = 0;
				*buffer = 1;
			}
		}
	}
}

int	cmds_til_pipe(char **cmds)
{
	int	i;

	i = -1;
	while (cmds[++i])
	{
		if (cmds[i][0] == '|')
			break ;
	}
	return (i);
}

int	count_export_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

int	til_dollar_sign(char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '$')
		len++;
	return (len);
}

void	chexpand(t_line **line, char *env, int index, char *buffer)
{
	char	*buffer;
	char	*aux;
	char	*joiner;
	int		i;

	printf("cmds -> %s\nenv -> %s\n", (*line)->cmds[index], env);
	aux = ft_strdup(env + count_cmdlen(env) + 1);
	buffer = ft_strdup((*line)->cmds[index]);
	printf("aux -> %s\nbuffer -> %s\n", aux, buffer);
	free((*line)->cmds[index]);
	(*line)->cmds[index] = malloc(sizeof(char) * (ft_strlen(buffer) - (count_cmdlen(env) + 1) + ft_strlen(aux) + 1));
	i = -1;
	while (buffer[++i])
	{
		if (buffer[i] == '$')
		{
			joiner = ft_strdup((*line)->cmds[index]);
			free((*line)->cmds[index]);
			(*line)->cmds[index] = ft_strjoin(joiner, aux);
			i += count_cmdlen(env);
			j += ft_strlen(aux);
		}
		else
			(*line)->cmds[index][++j] = buffer[i];
	}
	free(aux);
	free(buffer);
}

void	expanding(t_line **line, t_env *env, int j, int index)
{
	int		i;
	char	*buffer;
	char	*aux;

	buffer = ft_strdup(&(*line)->cmds[index][j + 1]);
	i = -1;
	while (env->env[++i])
	{
		if (buffer[til_dollar_sign(buffer)] == '$' && \
			!ft_strncmp(env->env[i], buffer, ft_strlen(buffer) - \
			ft_strlen(&buffer[til_dollar_sign(buffer)])))
		{
			chexpand(line, env->env[i], index, buffer);
			aux = ft_strdup(buffer);
			free(buffer);
			buffer = ft_strdup(&aux[til_dollar_sign(aux) + 1]);
			i = -1;
		}
		else if (!ft_strncmp(env->env[i], buffer, count_export_len(buffer)))
			chexpand(line, env->env[i], index, buffer);
	}
	free(buffer);
}

int	check_dir(char **cmds, char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		if (!ft_strncmp(env[i], "PWD=", 4))
			break ;
	if (!chdir(cmds[0]))
	{
		chdir(env[i]);
		check_dir_error(cmds[0]);
		return (1);
	}
	return (0);
}
