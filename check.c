/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:42:32 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/26 09:16:24 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void	cut_quotes(t_line **line, int i, int len)
{
	char *temp;
 	int		k;
 	int		j;

	k = 0;
	j = -1;
	temp = malloc((len + 1) * sizeof(char));
	while ((*line)->cmds[i][++j])
	{
			if ((*line)->cmds[i][j] == '\"')
					j++;
			temp[k] = ((*line)->cmds[i][j]);
			k++;
	}
 	temp[k] = 0;
	free(((*line)->cmds[i]));
	((*line)->cmds[i]) = ft_strdup(temp);
 	free(temp);
}

void	change_quotes(t_line **line)
{
	int	i;
	int	j;
 	int	len;
 	i = -1;
 	while ((*line)->cmds[++i])
	{
 		len = 0;
 		j = -1;
 		while ((*line)->cmds[i][++j])
 		{
 			if ((*line)->cmds[i][j] == '\"')
 			{
 				j++;
 				while ((*line)->cmds[i][j + len] && (*line)->cmds[i][j + len] != '\"')
 					len++;
 			}
 			if ((*line)->cmds[i][j + 1] == 0 && len > 0)
 				cut_quotes(line, i, len);
 		}
 	}
}
*/
int	check_quotes(char *cmd)
{

	int		i;
	int		len;
	char	q;

	i = -1;
	len = ft_strlen(cmd);
	while (cmd[++i])
	{
		
		if (cmd[i] == '"' || cmd[i] == '\'')
		{
			q = cmd[i];
			while (--len > i)
			{
				if (cmd[len] == q)
					break ;
				else if (len - 1 == i)
				{
					print_error("ERROR: unclosed quotes\n");
					return (0);
				}
			}
			if (ft_strlen(cmd) == 1)
			{
				print_error("ERROR: unclosed quotes\n");
				return (0);
			}
		}
	}
	return (1);
}

int	organize_line(t_line **line)
{
	char	**split_line;
	void	*head;

	if (!(*line)->cmd[0])
		return (0);
	head = *line;
	if (!check_line(*line) || !check_quotes((*line)->cmd))
		return (0);
	check_space(line);
	split_line = ft_split((*line)->cmd, ' ');
	init_cmds(line, split_line);
	check_for_pipes(line, (*line)->cmds);
	*line = head;
	init_files(line);
	*line = head;
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
					a_p[j++] = ft_strdup(cmds[i]);
				break ;
			}
			b_p[i] = ft_strdup(cmds[i]);
		}
		b_p[cmds_til_pipe(cmds)] = 0;
		a_p[j] = 0;
		//if (cmds_count(a_p) >= 1)
		init_linked_list(line, b_p, a_p);
	}
	//free_charpp(a_p);
	//free_charpp(b_p);
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
				&& (*line)->cmd[i + 1] != '<') && (*line)->cmd[i + 1])
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

int	check_line(t_line *line)
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
