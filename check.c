/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:42:32 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/17 12:41:41 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void	check_quotes(t_line **line)
{
	int	i;
	int	j;

	i = 0;
	while ((*line)->cmds[i])
	{
		j = 0;
		while ((*line)->cmds[i][j])
		{
			if (*line cmd)
		}
	}
}
*/
int	organize_line(t_line **line)
{
	char	**split_line;
	void	*head;

	if (!(*line)->cmd)
		return (0);
	head = (*line);
	check_line(*line);
	check_space(line);
	split_line = ft_split((*line)->cmd, ' ');
	init_files(line, split_line);
	init_cmds(line, split_line);
//	check_quotes(line);
	check_for_pipes(line, (*line)->cmds);
	(*line) = head;
	free(split_line);
	return (1);
}

void	check_for_pipes(t_line **line, char **cmds)
{
	int		i;
	int		j;
	char	**b_p;
	char	**a_p;

	i = -1;
	j = 0;
	b_p = malloc((cmds_til_pipe(cmds) + 1) * sizeof(char *));
	a_p = malloc((cmds_count(cmds) - cmds_til_pipe(cmds) + 1) * sizeof(char *));
	if (cmds_count(cmds) != cmds_til_pipe(cmds))
	{
		while (cmds[++i])
		{
			if (cmds[i][0] == '|')
			{
				while (cmds[++i])
					a_p[j++] = cmds[i];
				break ;
			}
			b_p[i] = cmds[i];
		}
		b_p[cmds_til_pipe(cmds)] = 0;
		a_p[j] = 0;
		init_linked_list(line, b_p, a_p);
	}
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
