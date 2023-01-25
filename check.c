/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:42:32 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/25 12:38:02 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
char	*ft_new_trim(char *cmd)
{
	char	*temp;

	temp = NULL;
	if (cmd[0] == '\'' || cmd[ft_strlen(cmd) - 1] == '\'')
		temp = ft_strtrim(cmd, "\'");
	else if (cmd[0] == '\"' || cmd[ft_strlen(cmd) - 1] == '\"')
		temp = ft_strtrim(cmd, "\"");
	else
		temp = ft_strdup(cmd);
	if (temp)
	{
		free(cmd);
		cmd = temp;
	}
	return (cmd);
}
*/

int	check_quote_on(char input)
{
	static int	d_quote;
	static int	s_quote;

	if (input == 39 && !s_quote && !d_quote)
		s_quote++;
	else if (input == 39 && !d_quote)
		s_quote--;
	else if (input == '"' && !s_quote && !d_quote)
		d_quote++;
	else if (input == '"' && !s_quote)
		d_quote--;
	if (s_quote || d_quote)
		return (1);
	return (0);
}

int	organize_line(t_line **line)
{
	char	**split_line;
	void	*head;

	if (!(*line)->cmd[0])
		return (0);
	head = *line;
	if (!check_line(*line))
		return (0);
	(*line)->cmd = check_space((*line)->cmd);
	split_line = ft_split((*line)->cmd, ' ');
	if (!init_cmds(line, split_line))
		return (0);
	check_for_pipes(line, (*line)->cmds);
	*line = head;
	if (!init_files(line))
		return (0);
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
		{
			new_cmd[j++] = ' ';
		}
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
		check_quote_on(cmd[i]);
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
