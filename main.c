/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/05 12:53:16 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void) argv;
	if (argc != 1)
		return (print_error("This program do not accept arguments\n"));
	minishell(envp);
	return (0);
}

void	cmd_process(char *cmd, char **envp)
{
	int		pid;
	char	*path;
	char	**cmds;

	cmds = get_cmds(cmd);
	free(cmd);
	if (find_path(cmds[0]) != 0)
	{
		pid = fork();
		if (pid == -1)
			print_error("Error: no child process created\n");
		if (pid == 0)
		{
			path = find_path(cmds[0]);
			execve(path, cmds, envp);
		}
		waitpid(pid, NULL, 0);
	}
	free_str_splited(cmds);
}

static void	put_space(t_line *line, int x)
{
	int		i;
	int		j;
	int		len;
	char	*new_cmd;

	i = 0;
	j = 0;
	len = ft_strlen(line->cmd);
	new_cmd = malloc((len + 2) * sizeof(char));
	while (line->cmd[i])
	{
		if (x == j)
		{
			new_cmd[j++] = ' ';
		}
		else
		{
			new_cmd[j] = line->cmd[i++];
			j++;
		}
	}
	new_cmd[j] = 0;
	free(line->cmd);
	line->cmd = ft_strdup(new_cmd);
	free(new_cmd);
}

static int	check_space(t_line *line)
{
	int	i;

	i = 0;
	while (line->cmd[i])
	{
		if (line->cmd[i] == '<' && (line->cmd[i + 1] != ' ' \
				&& line->cmd[i + 1] != '<'))
			put_space(line, i + 1);
		else if ((line->cmd[i] != ' ' && line->cmd[i] != '>' \
				&& line->cmd[i + 1] == '>'))
			put_space(line, i + 1);
		else if (line->cmd[i] == '>' && (line->cmd[i + 1] != ' ' \
				&& line->cmd[i + 1] != '>' && line->cmd[i + 1] != 0))
			put_space(line, i + 1);
		else if ((line->cmd[i] == '|' && line->cmd[i + 1] != ' ') \
				|| (line->cmd[i] != ' ' && line->cmd[i + 1] == '|'))
			put_space(line, i + 1);
		else
			i++;
	}
	return (1);
}

static void	check_line(t_line *line)
{
	int	i;

	i = -1;
	while (line->cmd[++i])
	{
		if (line->cmd[i] == '|' && line->cmd[i + 1] == '|')
		{
			ft_putstr_fd("Error: Can't use || or &&\n", 2);
			exit(EXIT_FAILURE);
		}
		if (line->cmd[i] == '&' && line->cmd[i + 1] == '&')
		{
			ft_putstr_fd("Error: Can't use || or &&\n", 2);
			exit(EXIT_FAILURE);
		}
	}
}

static int organize_line(t_line *line)
{
	int		i;
	char	**split_line;

	i = -1;
	if (!line->cmd)
		return (0);
	check_line(line);
	check_space(line);
	split_line = ft_split(line->cmd, ' ');
	while (split_line[++i])
		printf("cmd[%d]--> %s\n", i, split_line[i]);

	return (1);
}

void	minishell(char **envp)
{
	t_line	line;

	while (1)
	{
		signals();
		line.cmd = readline("minishell % ");
		if (line.cmd)
			add_history(line.cmd);
		else
			break ;
		organize_line(&line);
		if (ft_strncmp(line.cmd, "exit", 5) == 0)
			break ;
		cmd_process(line.cmd, envp);
	}
}
