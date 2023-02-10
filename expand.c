/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 08:38:10 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/10 13:44:03 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_var(t_line **line, t_env *env)
{
	int		single_quote;
	void	*head;
	int		i;
	int		j;

	head = *line;
	while (*line)
	{
		single_quote = 0;
		j = -1;
		while ((*line)->cmds[++j])
		{
			if ((*line)->cmds[j][0] == '\'')
				single_quote = 1;
			smart_trim(line, j);
			i = -1;
			while ((*line)->cmds[j][++i])
			{

				if (!single_quote && (*line)->cmds[j][i] == '$')
				{
					if ((*line)->cmds[j][i + 1] == '?')
						question_mark(line, j);
					else if ((*line)->cmds[j][i + 1])
					{
						expanding(line, env, i, j);
						if ((*line)->cmds[j] == NULL)
							break ;
						i = -1;
					}
				}
			}
		}
		*line = (*line)->next;
	}
	*line = head;
}

void	question_mark(t_line **line, int index)
{
	int		i;
	int		j;
	int		k;
	char	*buffer;
	char	*aux;

	aux = ft_itoa(g_status);
	buffer = ft_strdup((*line)->cmds[index]);
	free((*line)->cmds[index]);
	(*line)->cmds[index] = malloc(sizeof(char) * (ft_strlen(buffer) - 2\
		+ ft_strlen(aux) + 1));
	j = -1;
	i = -1;
	while (buffer[++i])
	{
		if (!ft_strncmp(&buffer[i], "$?", 2))
		{
			i++;
			k = -1;
			while (aux[++k])
				(*line)->cmds[index][++j] = aux[k];
		}
		else
			(*line)->cmds[index][++j] = buffer[i];
	}
	(*line)->cmds[index][j + 1] = 0;
	free(aux);
	free(buffer);
}

void	expanding(t_line **line, t_env *env, int j, int index)
{
	int	env_posi;

	env_posi = search_varenv((*line)->cmds[index], env, j);
	if (env_posi == -1)
		chexpand(line, env, NULL, index);
	else
		chexpand(line, env, env->env[env_posi], index);
}

int	search_varenv(char *cmds, t_env *env, int j)
{
	char	*buffer;
	int		i;

	buffer = ft_strdup(&cmds[j + 1]);
	i = -1;
	while (env->env[++i])
	{
		if (buffer[til_dollar_sign(buffer)] == '$' && \
			!ft_strncmp(env->env[i], buffer, ft_strlen(buffer) - \
			ft_strlen(&buffer[til_dollar_sign(buffer)])) && \
			env->env[i][til_dollar_sign(buffer)] == '=')
		{
			free(buffer);
			return (i);
		}
		else if (!ft_strncmp(env->env[i], buffer, count_export_len(buffer)) \
			&& env->env[i][count_export_len(buffer)] == '=')
		{
			free(buffer);
			return (i);
		}
	}
	free(buffer);
	return (-1);
}

int	exp_malloc(char *buf)
{
    int len;
    int rm;

    len = ft_strlen(buf);
    rm = 0;
    if (buf[0] == '$')
        rm = til_dollar_sign(&buf[1]) + 1;
    else
	{
        rm = til_dollar_sign(&buf[til_dollar_sign(buf)] + 1);
        if (rm == 0)
            rm = 1;
    }
    len =  len - rm + 1;
	return (len);
}

void chexpand(t_line **line, t_env *l_env, char *env, int index)
{
	char	*buffer;
	char	*aux;
	int		i;
	int		j;
	int		dollar;

	if (!env)
	{
		buffer = ft_strdup((*line)->cmds[index]);
		free((*line)->cmds[index]);
		aux = malloc(sizeof(char) * exp_malloc(buffer));
		i = -1;
		j = -1;
		while (buffer[++i])
		{
			if (buffer[i] == '$' && check_varenv(l_env->env, &buffer[i + 1]))
				aux[++j] = buffer[i];
			else if (buffer[i] == '$' && buffer[i + 1])
				i += count_export_len(&buffer[i + 1]);
			else
				aux[++j] = buffer[i];
		}
		free(buffer);
		aux[++j] = '\0';
		(*line)->cmds[index] = ft_strdup(aux);
		if (j == 0)
		{
			free((*line)->cmds[index]);
			(*line)->cmds[index] = NULL;
		}
		free(aux);
		return ;
	}
	dollar = 0;
	aux = ft_strdup(&env[count_cmdlen(env) + 1]);
	buffer = ft_strdup((*line)->cmds[index]);
	free((*line)->cmds[index]);
	(*line)->cmds[index] = malloc(sizeof(char) * (ft_strlen(buffer) - count_cmdlen(env) + ft_strlen(aux)));
	i = -1;
	int k = -1;
	while (buffer[++i])
	{
		if (!dollar && buffer[i] == '$')
		{
			dollar = 1;
			j = -1;
			while (aux[++j])
				(*line)->cmds[index][++k] = aux[j];
			i += count_cmdlen(env);
		}
		else
			(*line)->cmds[index][++k] = buffer[i];
	}
	(*line)->cmds[index][k + 1] = 0;
	free(buffer);
	free(aux);
}

int	til_dollar_sign(char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '$')
		len++;
	return (len);
}

int	check_varenv(char **env, char *str)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], str, count_cmdlen(env[i])) && \
			env[i][count_export_len(str)] == '=')
			return (1);
	}
	return (0);
}
