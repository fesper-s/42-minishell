/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:41 by fesper-s          #+#    #+#             */
/*   Updated: 2022/12/31 17:33:25 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*cmd;
	char	*path;

	(void) argv;
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
		path = find_path(cmd, envp);
	}
	return (0);
}
