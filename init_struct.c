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

int	file_len(char **cmd)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (cmd[++i])
	{
		if (!ft_strncmp(cmd[i], ">>", 2) && cmd[i + 1])
			j += 2;
		else if ((cmd[i][0] == '>' || cmd[i][0] == '<') && (cmd[i + 1]))
			j += 2;
	}
	return (i - j);
}

void	ch_cmd_data(t_line **line, char ***buffer, int j)
{
	buffer[0][j + 1] = 0;
	free_charpp((*line)->cmds);
	(*line)->cmds = ft_strdupp(buffer[0]);
	free_charpp(buffer[0]);
	(*line) = (*line)->next;
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
		if (!check_operator(line, (*line)->cmds))
			return (0);
		i = -1;
		j = -1;
		buffer = malloc((file_len((*line)->cmds) + 1) * sizeof(char *));
		while ((*line)->cmds[++i])
		{
			if ((*line)->cmds[i][0] == '<' && (*line)->cmds[i + 1])
				(*line)->infile = ft_strdup((*line)->cmds[++i]);
			else if ((*line)->cmds[i][0] == '>' && (*line)->cmds[i + 1])
					(*line)->outfile = ft_strdup((*line)->cmds[++i]);
			else
				buffer[++j] = ft_strdup((*line)->cmds[i]);
		}
		ch_cmd_data(line, &buffer, j);
	}
	(*line) = head;
	return (1);
}

int	init_cmds(t_line **line, char **split)
{
	int	len;

	if (!split[0])
		return(0);
	len = cmds_count(split);
	if (split[0][0] == '|' || split[len - 1][0] == '|')
	{
		print_error("parse error near '|'\n");
		return (0);
	}
	(*line)->cmds = ft_strdupp(split);
	return (1);
}

void	init_linked_list(t_line **line, char **before_pipe, char **after_pipe)
{
	ft_lst_add_back(line, ft_lst_new(ft_strdupp(after_pipe), NULL, NULL));
	free_charpp((*line)->cmds);
	(*line)->cmds = ft_strdupp(before_pipe);
	free_charpp(before_pipe);
	free_charpp(after_pipe);
	(*line) = (*line)->next;
	check_for_pipes(line, (*line)->cmds);
}
