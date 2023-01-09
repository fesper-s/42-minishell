/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 10:42:52 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/09 10:20:15 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtins(char **cmds)
{
	int	i;

	if (!ft_strncmp(cmds[0], "echo", 5))
	{
		i = 0;
		while (cmds[++i])
		{
			if (ft_strncmp(cmds[i], "-n", 3))
				printf("%s", cmds[i]);
			if (cmds[i + 1] != NULL && ft_strncmp(cmds[i], "-n", 3))
				printf(" ");
		}
		if (ft_strncmp(cmds[1], "-n", 3))
			printf("\n");
		return (1);
	}
	if (!ft_strncmp(cmds[0], "cd", 3))
	{
		printf("Enter CD\n");
		return (1);
	}
	return (0);
}
