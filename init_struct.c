/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 12:47:47 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/06 12:47:47 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env(char **envp)
{
	char	**env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (envp[++i])
		env[i] = envp[i];
	env[i] = 0;
	return (env);
}

void	init_files(t_line **line, char **split)
{
	int	i;

	i = -1;
	while (split[++i] && split[i][0] != '|')
	{
		if (ft_strlen(split[i]) == 1 && split[i][0] == '<')
			(*line)->infile = split[i + 1];
		else if (ft_strlen(split[i]) == 1 && split[i][0] == '>')
			(*line)->outfile = split[i + 1];
	}
}

void	init_cmds(t_line **line, char **split)
{
	int	i;
	int	j;
	int	len;

	j = 0;
	i = -1;
	len = cmds_count(split);
	(*line)->cmds = malloc((len + 1) * sizeof(char *));
	while (split[++i])
	{
		(*line)->cmds[j] = split[i];
		j++;
	}
	(*line)->cmds[j] = 0;
}

void	init_linked_list(t_line **line, char **before_pipe, char **after_pipe)
{
	int i = -1;
	ft_lst_add_back(line, ft_lst_new(after_pipe, NULL, \
		NULL));
	free((*line)->cmds);
	(*line)->cmds = before_pipe;
	while ((*line)->cmds[++i])
		printf("cmds[%d]--> %s\n", i, (*line)->cmds[i]);
	(*line) = (*line)->next;
	check_for_pipes(line, (*line)->cmds);
}
