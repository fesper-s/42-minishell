/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:42:32 by gussoare          #+#    #+#             */
/*   Updated: 2023/02/01 09:13:18 by gussoare         ###   ########.fr       */
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

int check_files(char **cmds)
{
	int	infile;
	int outfile;
	int i;

	i = -1;
	infile = 0;
	outfile = 0;
	while (cmds[++i])
	{
		if (cmds[i][0] == '<')
			infile++;
		if (cmds[i][0] == '>')
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

int	check_operator(t_line **line, char **cmds)
{
	int		i;
	int 	j;
	char	**buffer;

	i = -1;
	while (cmds[++i])
	{
		if ((cmds[i][0] == '<' || cmds[i][0] == '>') && !cmds[i + 1])
		{
			print_error("zsh: parse error\n");
			return (0);
		}
		else if (!strncmp(cmds[i], "<<<", 3) || !strncmp(cmds[i], ">>>", 3))
		{
			print_error("Error: multiples '<' or '>' operator\n");
			return (0);
		}
		else if (!strncmp(cmds[i], ">>", 2) && cmds[i + 1])
			(*line)->extract_op = 1;
		else if (!strncmp(cmds[i], "<<", 2) && cmds[i + 1])
		{
			(*line)->insert_op = 1;
			buffer = malloc((cmds_count(cmds) * sizeof(char *)));
			j = -1;
			i = 0;
			while (cmds[i])
			{
				if (!ft_strncmp(cmds[i], "<<", 2))
					i++;
				else
				buffer[++j] = cmds[i++];
			}
			//buffer;
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
	check_for_pipes(line, (*line)->cmds);
	*line = head;
	if (!init_files(line))
		return (0);
	*line = head;
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
				a_p = malloc((cmds_count(cmds) - cmds_til_pipe(cmds)) * sizeof(char *));
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
		if (cmd[i] == '<' && (cmd[i + 1] != '<' && cmd[i + 1] != ' ' && cmd[i + 1])) 
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] != ' ' && cmd[i] != '<' && cmd[i + 1] == '<')
			cmd = put_space(cmd, i + 1);
		else if (cmd[i] == '>' && (cmd[i + 1] != '>' && cmd[i + 1] != ' ' && cmd[i + 1]))
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

int check_double_pipes(t_line *line)
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
