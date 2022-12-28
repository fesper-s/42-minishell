/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:41 by fesper-s          #+#    #+#             */
/*   Updated: 2022/12/28 17:11:27 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*cmd;

	(void) argv;
	(void) envp;
	ignore_signal();
	if (argc != 1)
		return (print_error("This program do not accept arguments\n"));
	while (1)
	{
		cmd = readline("minishell % ");
		if (cmd)
			add_history(cmd);
		if (ft_strncmp(cmd, "exit", 4) == 0)
			break ;
	}
	return (0);
}
