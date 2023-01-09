/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:42:32 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/09 14:36:49 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_for_pipes(t_line **line, char **cmds)
{
	int		i;
	int		j;
	int		n_cmds;
	char	**before_pipe;
	char	**after_pipe;

	n_cmds = cmds_count(cmds);
	i = -1;
	j = 0;
	before_pipe = malloc((cmds_until_pipe(cmds) + 1) * sizeof(char *));
	after_pipe = malloc((n_cmds - cmds_until_pipe(cmds) + 1) * sizeof(char *));
	while (cmds[++i])
	{
		if (cmds[i][0] == '|')
		{
			while (cmds[++i])
				after_pipe[j++] = cmds[i];
			break ;
		}
		before_pipe[i] = cmds[i];
	}
	before_pipe[cmds_until_pipe(cmds)] = 0;
	after_pipe[j] = 0;
	init_linked_list(line, before_pipe, after_pipe);
}

void	put_space(t_line **line, int x)
{
	int		i;
	int		j;
	int		len;
	char	*new_cmd;

	i = 0;
	j = 0;
	len = ft_strlen((*line)->cmd);
	new_cmd = malloc((len + 2) * sizeof(char));
	while ((*line)->cmd[i])
	{
		if (x == j)
		{
			new_cmd[j++] = ' ';
		}
		else
		{
			new_cmd[j] = (*line)->cmd[i++];
			j++;
		}
	}
	new_cmd[j] = 0;
	free((*line)->cmd);
	(*line)->cmd = ft_strdup(new_cmd);
	free(new_cmd);
}

int	check_space(t_line **line)
{
	int	i;

	i = 0;
	while ((*line)->cmd[i])
	{
		if ((*line)->cmd[i] == '<' && ((*line)->cmd[i + 1] != ' ' \
				&& (*line)->cmd[i + 1] != '<'))
			put_space(line, i + 1);
		else if (((*line)->cmd[i] != ' ' && (*line)->cmd[i] != '>' \
				&& (*line)->cmd[i + 1] == '>'))
			put_space(line, i + 1);
		else if ((*line)->cmd[i] == '>' && ((*line)->cmd[i + 1] != ' ' \
				&& (*line)->cmd[i + 1] != '>' && (*line)->cmd[i + 1] != 0))
			put_space(line, i + 1);
		else if (((*line)->cmd[i] == '|' && (*line)->cmd[i + 1] != ' ') \
				|| ((*line)->cmd[i] != ' ' && (*line)->cmd[i + 1] == '|'))
			put_space(line, i + 1);
		else
			i++;
	}
	return (1);
}

void	check_line(t_line *line)
{
	int	i;

	i = -1;
	while (line->cmd[++i])
	{
		if (line->cmd[i] == '|' && line->cmd[i + 1] == '|')
		{
			ft_putstr_fd("Error: Can't use || or &&\n", 2);
			exit(EXIT_FAILURE);
		}
		if (line->cmd[i] == '&' && line->cmd[i + 1] == '&')
		{
			ft_putstr_fd("Error: Can't use || or &&\n", 2);
			exit(EXIT_FAILURE);
		}
	}
}
