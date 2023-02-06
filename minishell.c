/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/01 13:37:53 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

int	expand_var(t_line **line, t_env *env)
{
	if (!(*line)->cmds[0])
		return (0);
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
			g_status = 127;
		}
		else
			expanding(line, env);
		g_status = 127;
		return (1);
	}
	return (0);
}

void insert_exec(t_line **line)
{
	char	*eof;

	if ((*line)->insert_op)
	{
		eof = ft_strdup((*line)->insert_op);
		while (1)
		{
			eof = readline("> ");
			if (!eof)
			{
				printf("\n");
				break ;
			}
			if (!ft_strncmp((*line)->insert_op, eof, ft_strlen((*line)->insert_op)))
				break ;
			if ((*line)->outfile)
				printf("%s", eof);
			free(eof);
		}
		free(eof);
	}
}

void	exec_cmds(t_line **line, pid_t pid, char *path, int *fd)
{
	static int	fdd;

	if (pid == 0)
	{
		if ((*line)->infile_id > 0)
			dup2((*line)->infile_id, 0);
		else
			dup2(fdd, 0);
		if ((*line)->next != 0)
			dup2(fd[1], 1);
		if ((*line)->outfile_id > 0)
			dup2((*line)->outfile_id, 1);
		close(fd[0]);
		if (!path || handle_builtins((*line)->cmds, line))
		{
			if (!path)
				ft_putchar_fd('0', 1);
			exit(EXIT_SUCCESS);
		}
		else
			execve(path, (*line)->cmds, (*line)->env);
	}
	else
	{	
		close(fd[1]);
		fdd = fd[0];
		(*line) = (*line)->next;
	}
	free(path);
}

void open_files(t_line **line)
{
	if ((*line)->infile)
		(*line)->infile_id = open((*line)->infile, O_RDONLY);
	if ((*line)->infile_id  == -1)
		printf("Error: no such file or directory: %s\n", (*line)->infile);
	if ((*line)->outfile)
	{
		if ((*line)->extract_op)
			(*line)->outfile_id  = open((*line)->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			(*line)->outfile_id  = open((*line)->outfile, O_WRONLY | O_CREAT | O_TRUNC , 0644);
	}
	if ((*line)->outfile_id  == -1)
		printf("Error: no such file or directory: %s\n", (*line)->outfile);
}

void	pipeline(t_line **line, int size)
{
	int		fd[2];
	pid_t	pid;
	char 	*path;

	while ((*line))
	{
		path = find_path(line);
		insert_exec(line);
		pipe(fd);
		pid = fork();
		if (pid == -1 && print_error("Error: fork\n"))
			break ;
		else
			exec_cmds(line, pid, path, fd);
	}
	while (size--)
		waitpid(-1, NULL, 0);
}

void	cmd_process(t_line **line, t_env **env)
{
	void	*head;
	int		size;
	int		expand;

	size = ft_lst_size((*line));
	head = (*line);
	expand = expand_var(line, *env);
	while (*line)
	{
		open_files(line);
		(*line)->env = ft_strdupp((*env)->env);
		*line = (*line)->next;
	}
	*line = head;
	if (!expand && !check_dir((*line)->cmds, (*line)->env))
	{
		g_status = 0;
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
		{			if (ft_strncmp(line->cmd, "exit", 5) == 0)
				break ;
			cmd_process(&line, &env);
			line = head;
			lst_free(&line);
		}
		free(line);
	}
	exiting(&line, &env);
}
