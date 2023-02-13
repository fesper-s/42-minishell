/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 12:11:07 by gussoare          #+#    #+#             */
/*   Updated: 2023/02/13 12:11:48 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ch_cmd_data(t_line **line, char ***buffer, int j)
{
	buffer[0][j + 1] = 0;
	free_charpp((*line)->cmds);
	(*line)->cmds = ft_strdupp(buffer[0]);
	free_charpp(buffer[0]);
	(*line) = (*line)->next;
}

void	open_files(t_line **line)
{
	if ((*line)->infile)
		(*line)->infile_id = open((*line)->infile, O_RDONLY);
	if ((*line)->infile_id == -1)
		printf("Error: no such file or directory: %s\n", (*line)->infile);
	if ((*line)->outfile)
	{
		if ((*line)->extract_op)
			(*line)->outfile_id = open((*line)->outfile, O_WRONLY | O_CREAT \
				| O_APPEND, 0644);
		else
			(*line)->outfile_id = open((*line)->outfile, O_WRONLY | O_CREAT \
				| O_TRUNC, 0644);
	}
	if ((*line)->outfile_id == -1)
		printf("Error: no such file or directory: %s\n", (*line)->outfile);
}

int	file_len(char **cmd)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	if (!cmd[0])
		return (0);
	while (cmd[++i])
	{
		if ((cmd[i][0] == '>' || (cmd[i][0] == '<' && !cmd[i][0])) && \
			(cmd[i + 1]))
			j += 2;
	}
	return (i - j);
}
