/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/03 07:49:34 by fesper-s         ###   ########.fr       */
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

void	minishell(char **envp)
{
	int		pid;
	char	*cmd;
	char	**cmds;
	char	*path;

	while (1)
	{
		signals();
		cmd = readline("minishell % ");
		if (cmd)
			add_history(cmd);
		else
			break ;
		if (ft_strncmp(cmd, "exit", 5) == 0)
			break ;
		if (find_path(cmd, envp) != 0)
		{
			pid = fork();
			if (pid == -1)
				print_error("Error: no child process created\n");
			if (pid == 0)
			{
				cmds = get_cmds(cmd);
				path = find_path(cmds[0], envp);
				execve(path, cmds, envp);
			}
			waitpid(pid, NULL, 0);
		}
	}
}
