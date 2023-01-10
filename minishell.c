/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/10 14:21:17 by fesper-s         ###   ########.fr       */
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

void	cmd_process(char *cmd, char **env)
{
	int		pid;
	int		isbuiltin;
	char	*path;
	char	**cmds;

	cmds = get_cmds(cmd);
	if (!cmd[0])
		return ;
	isbuiltin = handle_builtins(cmds, env);
	if (find_path(cmds[0]) && !isbuiltin)
	{
		g_status = 0;
		pid = fork();
		if (pid == -1)
			print_error("Error: no child process created\n");
		if (pid == 0)
		{
			path = find_path(cmds[0]);
			execve(path, cmds, env);
		}
		waitpid(pid, NULL, 0);
	}
	expand_var(cmds[0]);
	free_str_splited(cmds);
}

char	**get_env(char **envp)
{
	char	**env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (envp[++i])
		env[i] = envp[i];
	env[i] = 0;
	return (env);
}

void	minishell(char **envp)
{
	t_line	line;
	char	**env;
	
	env = get_env(envp);
	while (1)
	{
		
		signals();
		line.cmd = readline("minishell % ");
		if (line.cmd)
			add_history(line.cmd);
		else
		{
			printf("exit\n");
			break ;
		}
		organize_line(&line);
		if (ft_strncmp(line.cmd, "exit", 5) == 0)
		{
			printf("exit\n");
			break ;
		}
		cmd_process(line.cmd, env);
	}
}
