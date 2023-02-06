/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/06 12:47:54 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

void	question_mark(t_line **line, int index)
{
	int		i;
	int		j;
	int		k;
	char	*buffer;
	char	*aux;

	aux = ft_itoa(g_status);
	buffer = ft_strdup((*line)->cmds[0]);
	free((*line)->cmds[index]);
	(*line)->cmds[index] = malloc(sizeof(char) * (ft_strlen(buffer) \
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
				(*line)->cmds[index][++j] = aux[k];
		}
		else
			(*line)->cmds[index][++j] = buffer[i];
	}
	(*line)->cmds[index][j + 1] = 0;
	free(aux);
	free(buffer);
}

void	expand_var(t_line **line, t_env *env)
{
	int		single_quote;
	int		i;
	int		j;

	single_quote = 0;
	j = -1;
	while ((*line)->cmds[++j])
	{
		if ((*line)->cmds[j][0] == '\'')
			single_quote = 1;
		(*line)->cmds[j] = smart_trim((*line)->cmds[j]);
		i = -1;
		while ((*line)->cmds[j][++i])
		{
			if (!single_quote && (*line)->cmds[j][i] == '$')
			{
				if ((*line)->cmds[j][i + 1] == '?')
					question_mark(line, j);
				else if ((*line)->cmds[j][i + 1])
				{
					expanding(line, env, i, j);
					i = -1;
				}
			}
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
	while (*line)
	{
		(*line)->env = ft_strdupp((*env)->env);
		*line = (*line)->next;
	}
	*line = head;
	expand_var(line, *env);
	isbuiltin = handle_builtins((*line)->cmds, env);
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
