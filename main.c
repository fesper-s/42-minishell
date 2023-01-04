/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/04 13:01:26 by fesper-s         ###   ########.fr       */
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
	if (!cmd[0])
		return ;
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
	if (cmds[0][0] == '$')
	{
		if (cmds[0][1] == '?')
		{
			printf("minishell: %d", g_status);
			if (cmds[0][2] != 0)
				printf("%s", &cmds[0][2]);
			printf(": command not found\n");
			g_status = 127;
		}
		else if (getenv(&cmds[0][1]) != NULL)
			printf("minishell: %s\n", getenv(&cmds[0][1]));
	}
	free_str_splited(cmds);
}

void	minishell(char **envp)
{
	char	*cmd;

	while (1)
	{
		signals();
		cmd = readline("minishell % ");
		if (cmd)
			add_history(cmd);
		else
		{
			printf("exit\n");
			break ;
		}
		if (ft_strncmp(cmd, "exit", 5) == 0)
		{
			printf("exit\n");
			break ;
		}
		cmd_process(cmd, envp);
	}
}
