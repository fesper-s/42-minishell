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

int	check_varenv(t_line *line, char *str)
{
	int	i;

	i = -1;
	while (line->env[++i])
	{
		if (!ft_strncmp(line->env[i], str, count_cmdlen(line->env[i])))
			return (1);
	}
	return (0);
}

void chexpand(t_line **line, char *env, int index)
{
	char	*buffer;
	char	*aux;
	char	*joiner;
	int		i;
	int		j;

	if (!env)
	{
		buffer = ft_strdup((*line)->cmds[index]);
		if (buffer[0] == '$')
			aux = ft_calloc(sizeof(char), ft_strlen(&buffer[til_dollar_sign(buffer + 1) + 1]));
		else
			aux = ft_calloc(sizeof(char), til_dollar_sign(buffer));
		i = -1;
		j = -1;
		while (buffer[++i])
		{
			if (buffer[i] == '$' && check_varenv(*line, &buffer[i + 1]))
				aux[++j] = buffer[i];
			else if (buffer[i] == '$')
				i += til_dollar_sign(&buffer[i + 1]);
			else
				aux[++j] = buffer[i];
		}
		aux[++j] = 0;
		(*line)->cmds[index] = ft_strdup(aux);
		free(aux);
		free(buffer);
		return ;
	}
	//printf("cmds -> %s\nenv -> %s\n", (*line)->cmds[index], env);
	aux = ft_strdup(env + count_cmdlen(env) + 1);
	buffer = ft_strdup((*line)->cmds[index]);
	//printf("aux -> %s\nbuffer -> %s\n", aux, buffer);
	free((*line)->cmds[index]);
	(*line)->cmds[index] = malloc(sizeof(char) * (ft_strlen(buffer) - (count_cmdlen(env) + 1) + ft_strlen(aux) + 1));
	i = -1;
	j = -1;
	while (buffer[++i])
	{
		if (buffer[i] == '$')
		{
			joiner = ft_strdup((*line)->cmds[index]);
			//printf("joiner -> %s\n", joiner);
			free((*line)->cmds[index]);
			(*line)->cmds[index] = ft_strjoin(joiner, aux);
			//printf("aux -> %s\ncmds -> %s\n", aux, (*line)->cmds[index]);
			i += count_cmdlen(env);
			j += ft_strlen(aux);
			while (buffer[++i])
				(*line)->cmds[index][++j] = buffer[i];
			break ;
		}
		else
			(*line)->cmds[index][++j] = buffer[i];
	}
	free(aux);
	free(buffer);
}

int	search_varenv(t_line **line, t_env *env, int index, int j)
{
	char	*buffer;
	int		i;

	buffer = ft_strdup(&(*line)->cmds[index][j + 1]);
	i = -1;
	while (env->env[++i])
	{
		if (buffer[til_dollar_sign(buffer)] == '$' && \
			!ft_strncmp(env->env[i], buffer, ft_strlen(buffer) - \
			ft_strlen(&buffer[til_dollar_sign(buffer)])))
			return (i);
		else if (!ft_strncmp(env->env[i], buffer, count_export_len(buffer)))
			return (i);
	}
	return (-1);
}

void	expanding(t_line **line, t_env *env, int j, int index)
{
	int	env_posi;

	env_posi = search_varenv(line, env, index, j);
	if (env_posi == -1)
		chexpand(line, NULL, index);
	else
		chexpand(line, env->env[env_posi], index);
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
