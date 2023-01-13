/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/13 12:14:21 by fesper-s         ###   ########.fr       */
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

void	cmd_process(t_line **line, t_env **env)
{
	int		pid;
	int		isbuiltin;
	char	*path;

	if (!(*line)->cmds[0])
		return ;
	expand_var(line, *env);
	isbuiltin = handle_builtins((*line)->cmds, env);
	if (!isbuiltin && find_path((*line)->cmds[0]))
	{
		g_status = 0;
		pid = fork();
		if (pid == -1)
			print_error("Error: no child process created\n");
		if (pid == 0)
		{
			path = find_path((*line)->cmds[0]);
			execve(path, (*line)->cmds, (*env)->env);
		}
		waitpid(pid, NULL, 0);
	}
	//free_str_splited((*line)->cmds);
}

int	organize_line(t_line **line)
{
	//int		i;
	char	**split_line;
	void	*head;

	//i = -1;
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
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->env = get_env(envp);
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
		cmd_process(&line, &env);
		lst_free(&line);
	}
}
