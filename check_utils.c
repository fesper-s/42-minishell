/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 11:53:59 by gussoare          #+#    #+#             */
/*   Updated: 2023/02/14 13:49:54 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rm_insert_op(t_line **line, char **cmds, int i)
{
	char	**buffer;
	int		j;

	j = 0;
	buffer = NULL;
	buffer = malloc((cmds_count(cmds) - 1) * sizeof(char *));
	while (cmds[++i])
	{
		if (!ft_strncmp(cmds[i], "<<", 2))
			i += 2;
		if (!cmds[i])
			break ;
		buffer[j++] = ft_strdup(cmds[i]);
	}
	buffer[j] = 0;
	free_charpp((*line)->cmds);
	if (buffer[0])
		ft_lst_add_next(line, ft_lst_new(ft_strdupp(buffer)));
	free_charpp(buffer);
	(*line)->cmds = malloc(sizeof(char *));
	(*line)->cmds[0] = 0;
}

char	*put_space(char *cmd, int x)
{
	int		i;
	int		j;
	int		len;
	char	*new_cmd;

	i = 0;
	j = 0;
	len = ft_strlen(cmd);
	new_cmd = malloc((len + 2) * sizeof(char));
	while (cmd[i])
	{
		if (x == j)
			new_cmd[j++] = ' ';
		else
		{
			new_cmd[j] = cmd[i++];
			j++;
		}
	}
	new_cmd[j] = 0;
	free(cmd);
	cmd = ft_strdup(new_cmd);
	free(new_cmd);
	return (cmd);
}

int	check_quote_on(char *cmd)
{
	int	d_quote;
	int	s_quote;
	int	i;

	d_quote = 0;
	s_quote = 0;
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' && !s_quote && !d_quote)
			s_quote++;
		else if (cmd[i] == '\'' && !d_quote)
			s_quote--;
		else if (cmd[i] == '"' && !s_quote && !d_quote)
			d_quote++;
		else if (cmd[i] == '"' && !s_quote)
			d_quote--;
	}
	if (s_quote || d_quote)
	{
		print_error("Error: unclosed quotes\n");
		return (1);
	}
	return (0);
}

int	check_double_pipes(t_line *line)
{
	int	i;

	i = -1;
	while (line->cmd[++i])
	{
		if (line->cmd[i] == '|' && line->cmd[i + 1] == '|')
		{
			ft_putstr_fd("Error: Can't use || or &&\n", 2);
			return (0);
		}
		if (line->cmd[i] == '&' && line->cmd[i + 1] == '&')
		{
			ft_putstr_fd("Error: Can't use || or &&\n", 2);
			return (0);
		}
	}
	return (1);
}

int	cut_cmd(t_line **line)
{
	char	**split_line;

	if (!(*line)->cmd[0])
		return (0);
	if (!check_double_pipes(*line) || check_quote_on((*line)->cmd))
		return (0);
	(*line)->cmd = check_space((*line)->cmd);
	split_line = ft_split((*line)->cmd, ' ');
	if (!init_cmds(line, split_line) || !check_files((*line)->cmds))
	{
		free_charpp((*line)->cmds);
		free((*line)->cmd);
		free_charpp(split_line);
		return (0);
	}
	free_charpp(split_line);
	return (1);
}
