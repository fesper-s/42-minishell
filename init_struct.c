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
		env[i] = ft_strdup(envp[i]);
	env[i] = 0;
	return (env);
}

int	file_len(char **cmd)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (cmd[++i])
	{
		if ((cmd[i][0] == '>' || cmd[i][0] == '<') && (cmd[i + 1]))
			j += 2;
	}
	return (i - j);
}

int	init_files(t_line **line)
{
	int		i;
	int		j;
	void	*head;
	char	**buffer;

	head = (*line);
	while ((*line))
	{
		i = -1;
		j = 0;
		buffer = malloc((file_len((*line)->cmds) + 1) * sizeof(char *));
		while ((*line)->cmds[++i])
		{
			if ((*line)->cmds[i][0] == '<' && (*line)->cmds[i + 1])
				(*line)->infile = ft_strdup((*line)->cmds[++i]);
			else if ((*line)->cmds[i][0] == '>' && (*line)->cmds[i + 1])
			{
				if ((*line)->outfile)
					(*line)->outfile = ft_strdup((*line)->cmds[++i]);
			}
			else
			{
				buffer[j] = ft_strdup((*line)->cmds[i]);
				j++;
			}
		}
		buffer[j] = 0;
		free_charpp((*line)->cmds);
		i = -1;
		(*line)->cmds = malloc(sizeof(char *) * (cmds_count(buffer) + 1));
		while (buffer[++i])
			(*line)->cmds[i] = ft_strdup(buffer[i]);
		(*line)->cmds[i] = 0;
		free_charpp(buffer);
		(*line) = (*line)->next;
	}
	(*line) = head;
	return (1);
}

int	init_cmds(t_line **line, char **split)
{
	int	i;
	int	len;

	i = -1;
	len = cmds_count(split);
	if (split[0][0] == '|' || split[len - 1][0] == '|')
	{
		print_error("parse error near '|'\n");
		return (0);
	}
	(*line)->cmds = malloc((len + 1) * sizeof(char *));
	while (split[++i])
		(*line)->cmds[i] = ft_strdup(split[i]);
	(*line)->cmds[i] = 0;
	return (1);
}

void	init_linked_list(t_line **line, char **before_pipe, char **after_pipe)
{
	int	i;

	i = -1;
	ft_lst_add_back(line, ft_lst_new(ft_strdupp(after_pipe), NULL, NULL));
	free_charpp((*line)->cmds);
	(*line)->cmds = malloc((cmds_count(before_pipe) + 1) * sizeof(char *));
	while (before_pipe[++i])
		(*line)->cmds[i] = ft_strdup(before_pipe[i]);
	(*line)->cmds[i] = 0;
	free_charpp(before_pipe);
	free_charpp(after_pipe);
	(*line) = (*line)->next;
	check_for_pipes(line, (*line)->cmds);
}
