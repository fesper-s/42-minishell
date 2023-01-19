/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/19 10:08:59 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_var(t_line **line, t_env *env)
{
	int		i;
	int		j;
	char	*buffer;

	if ((*line)->cmds[0][0] == '$')
	{
		if (!(*line)->cmds[0][1])
			cmd_error((*line)->cmds[0]);
		else if ((*line)->cmds[0][1] == '?')
		{
			printf("minishell: %d", g_status);
			if ((*line)->cmds[0][2] != 0)
				printf("%s", &(*line)->cmds[0][2]);
			printf(": command not found\n");
		}
		else
		{
			i = -1;
			while (env->env[++i])
			{
				if (!ft_strncmp(env->env[i], &(*line)->cmds[0][1], \
					ft_strlen(&(*line)->cmds[0][1])))
				{
					j = 0;
					while (env->env[i][j] != '=')
						j++;
					buffer = malloc(sizeof(char) * (ft_strlen(env->env[i]) - j + 1));
					buffer = ft_strdup(env->env[i] + j + 1);
					free((*line)->cmds[0]);
					(*line)->cmds[0] = ft_strdup(buffer);
				}
			}
		}
		g_status = 127;
	}
}

void	exec_cmds(t_line **line, pid_t pid, int *fdd, int *fd)
{
	char	*path;

	path = find_path((*line)->cmds[0]);
	if (pid == 0)
	{
		dup2(*fdd, 0);
		if ((*line)->next != 0)
			dup2(fd[1], 1);
		close(fd[0]);
		execve(path, (*line)->cmds, (*line)->env->env);
	}
	else
	{
		close(fd[1]);
		*fdd = fd[0];
		(*line) = (*line)->next;
	}
	free(path);
}

void	pipeline(t_line **line, int size)
{
	int		fd[2];
	pid_t	pid;
	int		fdd;
	char	*path;

	fdd = 0;
	while ((*line))
	{
		path = find_path((*line)->cmds[0]);
		if (!path)
			break ;
		pipe(fd);
		pid = fork();
		if (pid == -1 && print_error("Error: fork\n"))
			break ;
		else
			exec_cmds(line, pid, &fdd, fd);
		free(path);
	}
	while (size--)
		waitpid(-1, NULL, 0);
}

void	cmd_process(t_line **line, t_env **env)
{
	int		isbuiltin;
	void	*head;
	int		size;

	size = ft_lst_size((*line));
	head = (*line);
	if (!(*line)->cmds[0])
		return ;
	expand_var(line, *env);
	isbuiltin = handle_builtins((*line)->cmds, env);
	(*line)->env = *env;
	if (!isbuiltin && find_path((*line)->cmds[0]))
	{
		g_status = 0;
		pipeline(line, size);
		(*line) = head;
	}
	free_charpp((*line)->cmds);
}

void	minishell(char **envp)
{
	t_line	*line;
	t_env	*env;
	void	*head;

	env = malloc(sizeof(t_env));
	env->env = get_env(envp);
	while (1)
	{
		line = ft_lst_new(NULL, NULL, NULL);
		head = line;
		signals();
		line->cmd = readline("minishell % ");
		if (line->cmd)
			add_history(line->cmd);
		else
			break ;
		if (organize_line(&line))
		{
			if (ft_strncmp(line->cmd, "exit", 5) == 0)
				break ;
			cmd_process(&line, &env);
			line = head;
			lst_free(&line);
		}
	}
	printf("exit\n");
}
