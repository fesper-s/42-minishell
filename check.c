/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:42:32 by gussoare          #+#    #+#             */
/*   Updated: 2023/02/13 15:38:23 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	check_builtins(t_line **line, t_env **env, int size)
{
	if ((*line)->cmds[0] && size == 1)
	{
		if (!ft_strncmp((*line)->cmds[0], "export", 7))
			handle_export((*line)->cmds, env);
		if (!ft_strncmp((*line)->cmds[0], "unset", 6))
			handle_unset((*line)->cmds, env);
		if (!ft_strncmp((*line)->cmds[0], "cd", 3))
			handle_cd((*line)->cmds, env);
	}
}

void	check_for_pipes(t_line **line, char **cmd)
{
	int		i;
	int		j;
	char	**b_p;
	char	**a_p;

	i = -1;
	j = 0;
	b_p = NULL;
	a_p = NULL;
	if (cmds_count(cmd) != tilpipe(cmd))
	{
		b_p = malloc((tilpipe(cmd) + 1) * sizeof(char *));
		while (cmd[++i])
		{
			if (cmd[i][0] == '|')
			{
				a_p = malloc((cmds_count(cmd) - tilpipe(cmd)) * sizeof(char *));
				while (cmd[++i])
					a_p[j++] = ft_strdup(cmd[i]);
				break ;
			}
			b_p[i] = ft_strdup(cmd[i]);
		}
		b_p[tilpipe(cmd)] = 0;
		a_p[j] = 0;
		init_linked_list(line, b_p, a_p);
	}
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
