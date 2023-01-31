/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/31 11:13:21 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

void	question_mark(t_line **line)
{
	int		i;
	int		j;
	int		k;
	char	*buffer;
	char	*aux;

	aux = ft_itoa(g_status);
	buffer = ft_strdup((*line)->cmds[0]);
	free((*line)->cmds[0]);
	(*line)->cmds[0] = malloc(sizeof(char) * (ft_strlen(buffer) \
		- 2 + ft_strlen(aux) + 1));
	j = -1;
	i = -1;
	while (buffer[++i])
	{
		if (!ft_strncmp(&buffer[i], "$?", 2))
		{
			i += 2;
			k = -1;
			while (aux[++k])
				(*line)->cmds[0][++j] = aux[k];
		}
		else
		{
			printf("e aqui será que entra?\n");
			(*line)->cmds[0][++j] = buffer[i];
		}
	}
	(*line)->cmds[0][j + 1] = 0;
	free(aux);
	free(buffer);
}

void	expand_var(t_line **line, t_env *env)
{
	int		single_quote;
	int		i;

	single_quote = 0;
	if ((*line)->cmds[0][0] == '\'')
		single_quote = 1;
	(*line)->cmds[0] = smart_trim((*line)->cmds[0]);
	i = -1;
	while ((*line)->cmds[0][++i])
	{
		if (!single_quote && (*line)->cmds[0][i] == '$')
		{
			if ((*line)->cmds[0][i + 1] == '?')
				question_mark(line);
			else if ((*line)->cmds[0][i + 1])
				expanding(line, env);
			g_status = 127;
		} 
	}
}

void	exec_cmds(t_line **line, pid_t pid, int *fdd, int *fd)
{
	char	*path;

	path = find_path(line);
	if (pid == 0)
	{
		dup2(*fdd, 0);
		if ((*line)->next != 0)
			dup2(fd[1], 1);
		close(fd[0]);
		execve(path, (*line)->cmds, (*line)->env);
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
		path = find_path(line);
		if (!path)
			break ;
		pipe(fd);
		pid = fork();
		if (pid == -1 && print_error("Error: fork\n"))
			break ;
		else
		{
			exec_cmds(line, pid, &fdd, fd);
			g_status = 0;
		}
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
	while (*line)
	{
		(*line)->env = ft_strdupp((*env)->env);
		*line = (*line)->next;
	}
	*line = head;
	if (!isbuiltin && !check_dir((*line)->cmds, (*env)->env))
	{
		pipeline(line, size);
		(*line) = head;
	}
}

void	minishell(char **envp)
{
	t_line	*line;
	t_env	*env;
	void	*head;

	env = malloc(sizeof(t_env));
	env->env = ft_strdupp(envp);
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
		free(line);
	}
	exiting(&line, &env);
}
