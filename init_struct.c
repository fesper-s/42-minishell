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
		j = -1;
		buffer = malloc((file_len((*line)->cmds) + 1) * sizeof(char *));
		while ((*line)->cmds[++i])
		{
			if ((*line)->cmds[i][0] == '<' && (*line)->cmds[i + 1] \
				&& (*line)->cmds[i][1] != '<')
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
		return (0);
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
	ft_lst_add_back(line, ft_lst_new(ft_strdupp(after_pipe)));
	free_charpp((*line)->cmds);
	(*line)->cmds = ft_strdupp(before_pipe);
	free_charpp(before_pipe);
	free_charpp(after_pipe);
	(*line) = (*line)->next;
	check_for_pipes(line, (*line)->cmds, -1, 0);
}
