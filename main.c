/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:41 by fesper-s          #+#    #+#             */
/*   Updated: 2022/12/30 18:11:41 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*cmd;

	(void) argv;
	(void) envp;
	if (argc != 1)
		return (print_error("This program do not accept arguments\n"));
	while (1)
	{
		signals();
		cmd = readline("minishell % ");
		if (cmd)
			add_history(cmd);
		else
			break ;
		if (ft_strncmp(cmd, "exit", 4) == 0)
			break ;
	}
	return (0);
}
