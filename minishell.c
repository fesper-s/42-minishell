/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/10 14:40:27 by gussoare         ###   ########.fr       */
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

void	cmd_process(t_line **line, char **env)
{
	int		pid;
	int		isbuiltin;
	char	*path;

	if (!(*line)->cmds)
		return ;
	isbuiltin = handle_builtins((*line)->cmds, env);
	if (find_path((*line)->cmds[0]) && !isbuiltin)
	{
		g_status = 0;
		pid = fork();
		if (pid == -1)
			print_error("Error: no child process created\n");
		if (pid == 0)
		{
			path = find_path((*line)->cmds[0]);
			execve(path, (*line)->cmds, env);
		}
		waitpid(pid, NULL, 0);
	}
	expand_var((*line)->cmds[0]);
	free_str_splited((*line)->cmds);
}

int	organize_line(t_line **line)
{
	int		i;
	char	**split_line;
	void	*head;

	i = -1;
	if (!(*line)->cmd)
		return (0);
	head = (*line);
	check_line(*line);
	check_space(line);
	split_line = ft_split((*line)->cmd, ' ');
	init_files(line, split_line);
	init_cmds(line, split_line);
	check_for_pipes(line, (*line)->cmds);
	(*line) = head;
	/*
	printf("-----Init Infile and Outfile-----\n");
	if ((*line)->infile)
		printf("infile--> %s\n", (*line)->infile);
	if ((*line)->outfile)
		printf("outfile--> %s\n", (*line)->outfile);
	i = -1;
	printf("-----first list-----\n");
	while ((*line))
	{
		while ((*line)->cmds[++i])
			printf("cmds[%d]--> %s\n", i, (*line)->cmds[i]);
		i = -1;
		if ((*line)->next)
			printf("-----Next list-----\n");
		(*line) = (*line)->next;
	}
	(*line) = head;
	*/
	free(split_line);
	return (1);
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
	t_line	*line;
	char	**env;

	env = get_env(envp);
	while (1)
	{
		line = ft_lst_new(NULL, NULL, NULL);
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
		//printf("-----Starting CMD Process\n");
		cmd_process(&line, env);
		free(line->cmd);
	}
}
