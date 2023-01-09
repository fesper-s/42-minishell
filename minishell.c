/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/09 14:34:29 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_var(char *cmd)
{
	if (cmd[0] == '$')
	{
		if (!cmd[1])
			cmd_error(cmd);
		if (cmd[1] == '?')
		{
			printf("minishell: %d", g_status);
			if (cmd[2] != 0)
				printf("%s", &cmd[2]);
			printf(": command not found\n");
		}
		else if (getenv(&cmd[1]) != NULL)
			printf("minishell: %s\n", getenv(&cmd[1]));
		g_status = 127;
	}
}

void	cmd_process(char *cmd, char **envp)
{
	int		pid;
	int		isbuiltin;
	char	*path;
	char	**cmds;

	cmds = get_cmds(cmd);
	if (!cmd[0])
		return ;
	isbuiltin = handle_builtins(cmds);
	if (find_path(cmds[0]) && !isbuiltin)
	{
		g_status = 0;
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
	expand_var(cmds[0]);
	free_str_splited(cmds);
}

int	organize_line(t_line **line)
{
	int		i;
	char	**split_line;

	i = -1;
	if (!(*line)->cmd)
		return (0);
	init_values(line);
	check_line(*line);
	check_space(line);
	split_line = ft_split((*line)->cmd, ' ');
	init_files(line, split_line);
	init_cmds(line, split_line);
	check_for_pipes(line, (*line)->cmds);
	printf("-----Spliting CMD-----\n");
	while (split_line[++i])
		printf("cmd[%d]--> %s\n", i, split_line[i]);
	i = -1;
	printf("-----Getting CMDS-----\n");
	while ((*line)->cmds[++i])
		printf("cmds[%d]--> %s\n", i, (*line)->cmds[i]);
	printf("-----Init Infile and Outfile-----\n");
	if ((*line)->infile)
		printf("infile--> %s\n", (*line)->infile);
	if ((*line)->outfile)
		printf("outfile--> %s\n", (*line)->outfile);
	free(split_line);
	return (1);
}

void	minishell(char **envp)
{
	t_line	*line;

	while (1)
	{
		signals();
		line->cmd = readline("minishell % ");
		if (line->cmd)
			add_history(line->cmd);
		else
		{
			printf("exit\n");
			break ;
		}
		organize_line(&line);
		if (ft_strncmp(line->cmd, "exit", 5) == 0)
		{
			printf("exit\n");
			break ;
		}
		printf("-----Starting CMD Process\n");
		cmd_process(line->cmd, envp);
		free(line->cmd);
	}
}
