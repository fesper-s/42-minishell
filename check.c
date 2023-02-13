/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:42:32 by gussoare          #+#    #+#             */
/*   Updated: 2023/02/13 09:14:38 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	check_files(char **cmds)
{
	int	infile;
	int	outfile;
	int	i;

	i = -1;
	infile = 0;
	outfile = 0;
	while (cmds[++i])
	{
		if (cmds[i][0] == '<' && cmds[i][1] != '<')
			infile++;
		if (cmds[i][0] == '>' && cmds[i][1] != '>')
			outfile++;
		if (infile > 1 || outfile > 1)
		{
			print_error("Error: Multiples files\n");
			return (0);
		}
		if (cmds[i][0] == '|')
		{
			infile = 0;
			outfile = 0;
		}
	}
	return (1);
}

void	ft_lst_add_next(t_line **lst, t_line *new)
{
	void	*next;

	if (!lst)
		return ;
	next = (*lst)->next;
	(*lst)->next = new;
	new->next = next;
}

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

void	check_heredocs(t_line **line, char **cmds, int *i)
{
	if (!ft_strncmp(cmds[*i], ">>", 2) && cmds[*i + 1])
			(*line)->extract_op = 1;
	else if (!ft_strncmp(cmds[*i], "<<", 2) && cmds[*i + 1])
	{
		(*line)->insert_op = ft_strdup(cmds[*i + 1]);
		*i = -1;
		if (cmds_count(cmds) > 2)
			rm_insert_op(line, cmds, *i);
		else
		{
			free_charpp((*line)->cmds);
			(*line)->cmds = ft_calloc(1, sizeof(char *));
		}
		*i = -1;
	}
}

int	check_operator(t_line **line, char **cmds)
{
	int		i;

	i = -1;
	while (cmds[++i])
	{
		if ((cmds[i][0] == '<' || cmds[i][0] == '>') && !cmds[i + 1])
		{
			print_error("zsh: parse error\n");
			return (0);
		}
		else if ((cmds[i][0] == '<' || cmds[i][0] == '>') && \
			(cmds[i + 1][0] == '<' || cmds[i + 1][0] == '>'))
		{
			print_error("zsh: parse error\n");
			return (0);
		}
		else if (!ft_strncmp(cmds[i], "<<<", 3) || \
			!ft_strncmp(cmds[i], ">>>", 3))
		{
			print_error("Error: multiples '<' or '>' operator\n");
			return (0);
		}
		check_heredocs(line, cmds, &i);
	}
	return (1);
}

int cut_cmd(t_line **line)
{
	char **split_line;

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

int	organize_line(t_line **line)
{
	void	*head;

	head = (*line);
	if (!cut_cmd(line))
		return (0);
	check_for_pipes(line, (*line)->cmds);
	*line = head;
	while ((*line))
	{
		if (!check_operator(line, (*line)->cmds))
			return (0);
		(*line) = (*line)->next;
	}
	*line = head;
	if (!init_files(line))
		return (0);
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
	a_p = NULL;
	b_p = NULL;
	if (cmds_count(cmds) != cmds_til_pipe(cmds))
	{
		b_p = malloc((cmds_til_pipe(cmds) + 1) * sizeof(char *));
		while (cmds[++i])
		{
			if (cmds[i][0] == '|')
			{
				a_p = malloc((cmds_count(cmds) - cmds_til_pipe(cmds)) * \
					sizeof(char *));
				while (cmds[++i])
					a_p[j++] = ft_strdup(cmds[i]);
				break ;
			}
			b_p[i] = ft_strdup(cmds[i]);
		}
		b_p[cmds_til_pipe(cmds)] = 0;
		a_p[j] = 0;
		init_linked_list(line, b_p, a_p);
	}
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

char	*check_space(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '<' && (cmd[i + 1] != '<' && cmd[i + 1] != ' ' && \
			cmd[i + 1]))
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] != ' ' && cmd[i] != '<' && cmd[i + 1] == '<')
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] == '>' && (cmd[i + 1] != '>' && cmd[i + 1] != ' ' \
			&& cmd[i + 1]))
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] != ' ' && cmd[i] != '>' && cmd[i + 1] == '>')
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] == '|' && cmd[i + 1] != ' ' && cmd[i + 1])
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] != ' ' && cmd[i + 1] == '|')
			cmd = put_space(cmd, i + 1);
		else
			i++;
	}
	return (cmd);
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
