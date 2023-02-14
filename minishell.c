/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 13:51:41 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/14 09:00:52 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

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

int	organize_line(t_line **line)
{
	void	*head;

	head = (*line);
	if (!cut_cmd(line))
		return (0);
	check_for_pipes(line, (*line)->cmds, -1, 0);
	*line = head;
	while ((*line))
	{
		if (!check_operator(line, (*line)->cmds))
			return (0);
		(*line) = (*line)->next;
	}
	*line = head;
	if (!init_files(line))
		return (0);
	return (1);
}

void	minishell(char **envp)
{
	t_line	*line;
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->env = ft_strdupp(envp);
	while (1)
	{
		line = ft_lst_new(NULL);
		signals();
		line->cmd = readline("minishell % ");
		if (line->cmd)
			add_history(line->cmd);
		else
			break ;
		if (organize_line(&line))
		{
			if (!ft_strncmp(line->cmds[0], "exit", 5))
				break ;
			cmd_process(&line, &env);
			lst_free(&line);
		}
		free(line);
	}
	exiting(&line, &env);
}
