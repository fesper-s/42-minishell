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

void	init_files(t_line **line, char **split)
{
	int	i;

	i = -1;
	while (split[++i])
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
	if ((*line)->infile)
	{
		len -= 2;
		i = 1;
	}
	else if ((*line)->outfile)
		len -= 2;
	(*line)->cmds = malloc((len + 1) * sizeof(char *));
	while (split[++i])
	{
		if (split[i][0] == '>')
			break ;
		(*line)->cmds[j] = split[i];
		j++;
	}
	(*line)->cmds[j] = 0;
}

void	init_linked_list(t_line **line, char **before_pipe, char **after_pipe)
{
	ft_lst_add_back(line, ft_lst_new(after_pipe, (*line)->infile, \
			(*line)->outfile));
	free((*line)->cmds);
	(*line)->cmds = before_pipe;
	(*line) = (*line)->next;
	check_for_pipes(line, (*line)->cmds);
}
