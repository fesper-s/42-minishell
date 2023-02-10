/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/10 16:41:51 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

void	insert_operation(t_line **line, char *eof)
{
	int		i;
	char	**buffer;

	i = -1;
	buffer = NULL;
	if (!(*line)->insert_char)
	{
		(*line)->insert_char = malloc(2 * sizeof(char *));
		(*line)->insert_char[0] = ft_strdup(eof);
		(*line)->insert_char[1] = 0;
	}
	else
	{
		buffer = malloc((cmds_count((*line)->insert_char) + 2) * sizeof(char *));
		while ((*line)->insert_char[++i])
			buffer[i] = ft_strdup((*line)->insert_char[i]);
		buffer[i] = ft_strdup(eof);
		buffer[i + 1] = 0;
		free_charpp((*line)->insert_char);
		(*line)->insert_char = ft_strdupp(buffer);
		free_charpp(buffer);
	}
}

void insert_exec(t_line **line)
{
	char	*eof;

	if ((*line)->insert_op)
	{
		while (1)
		{
			eof = readline("> ");
			if (!eof[0])
			{
				printf("\n");
				break ;
			}
			if (!ft_strncmp((*line)->insert_op, eof, ft_strlen((*line)->insert_op)) && ft_strlen((*line)->insert_op) == ft_strlen(eof))
				break ;
			insert_operation(line, eof);
			free(eof);
		}
		free(eof);
	}
}

void	print_insert(t_line **line)
{
	int i;
	i = -1;
	if ((*line)->insert_char && (*line)->next)
	{		
		while ((*line)->insert_char[++i])
			printf("%s\n", (*line)->insert_char[i]);
	}
}
void	exec_cmds(t_line **line, t_env **env, int *fd, int *fdd)
{
	if ((*line)->child == 0)
	{
		if ((*line)->infile_id > 0)
			dup2((*line)->infile_id, 0);
		else
			dup2(*fdd, 0);
		if ((*line)->next != 0)
			dup2(fd[1], 1);
		if ((*line)->outfile_id > 0)
			dup2((*line)->outfile_id, 1);
		close(fd[0]);
		if (check_dir((*line)->cmds, (*env)->env) || handle_builtins\
			((*line)->cmds, env) || !(*line)->path)
		{
			print_insert(line);
			if (!(*line)->path)
				printf("");
			exit(EXIT_SUCCESS);
		}
		else
			execve((*line)->path, (*line)->cmds, (*env)->env);
	}
	else
	{
		close(fd[1]);
		*fdd = fd[0];
		(*line) = (*line)->next;
	}
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

void	pipeline(t_line **line, t_env **env, int size)
{
	int	fd[2];
	int	fdd;

	fdd = 0;
	while ((*line))
	{
		(*line)->path = find_path(line, env);
		insert_exec(line);
		pipe(fd);
		(*line)->child = fork();
		if ((*line)->child == -1 && print_error("Error: fork\n"))
			break ;
		else
		{
			exec_cmds(line, env, fd, &fdd);
			g_status = 0;
		}
	}
	while (size--)
		waitpid(-1, NULL, 0);
}

void	check_builtins(t_line **line, t_env **env, int size)
{
	if ((*line)->cmds[0] && size == 1)
	{
		if (!ft_strncmp((*line)->cmds[0], "export", ft_strlen((*line)->cmds[0])))
			handle_export((*line)->cmds, env);
		if (!ft_strncmp((*line)->cmds[0], "unset", ft_strlen((*line)->cmds[0])))
			handle_unset((*line)->cmds, env);
		if (!ft_strncmp((*line)->cmds[0], "cd", ft_strlen((*line)->cmds[0])))
			handle_cd((*line)->cmds, env);
	}
}


void	cmd_process(t_line **line, t_env **env)
{
	void	*head;
	int		size;

	size = ft_lst_size((*line));
	head = (*line);
	while (*line)
	{
		open_files(line);
		*line = (*line)->next;
	}
	*line = head;
	expand_var(line, *env, 0);
	check_builtins(line, env, size);
	pipeline(line, env, size);
	(*line) = head;
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
		line = ft_lst_new(NULL);
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
