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

void	expanding(t_line **line, t_env *env, int index)
{
	char	*buffer;
	char	*aux;
	int		i;
	int		j;
	int		k;
	int		teste;

	buffer = ft_strdup(&(*line)->cmds[0][index + 1]);
	i = -1;
	while (env->env[++i])
	{
		j = -1;
		while (buffer[++j])
		{
			if (buffer[j] == '$')
				break ;
		}
		if (buffer[j] == '$')
		{
			if (!ft_strncmp(env->env[i], buffer, \
				ft_strlen(buffer) - ft_strlen(&buffer[j])))
				break ;
		}
		else
		{
			if (!ft_strncmp(env->env[i], buffer, ft_strlen(buffer)))
				break ;
		}
	}
	free(buffer);
	if (env->env[i])
	{
		j = 0;
		while (env->env[i][j] != '=')
			j++;
		aux = ft_strdup(env->env[i] + j + 1);
		buffer = ft_strdup((*line)->cmds[0]);
		free((*line)->cmds[0]);
		teste = 0;
		while (buffer[teste] != '$')
			teste++;
		(*line)->cmds[0] = malloc(sizeof(char) * (teste + ft_strlen(aux) + 1));
		teste = -1;
		k = -1;
		while (buffer[++teste])
		{
			if (buffer[teste] != '$')
				(*line)->cmds[0][++k] = buffer[teste];
			else
			{
				free(buffer);
				buffer = ft_strdup((*line)->cmds[0]);
				free((*line)->cmds[0]);
				(*line)->cmds[0] = ft_strjoin(buffer, aux);
				break ;
			}
		}
		free(aux);
		free(buffer);
	}
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
