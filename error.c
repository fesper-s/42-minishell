/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 14:52:21 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/05 12:38:01 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error(char *str)
{
	ft_putstr_fd(str, 2);
	return (1);
}

void	cmd_error(char *cmd)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putchar_fd('\n', 2);
}

void	error_display(char *cmd)
{
	if (cmd[0] != '$')
	{
		cmd_error(cmd);
		g_status = 127;
	}
}

char	*path_error(char *cmd, char **path)
{
	if (cmd[0] != '$')
	{
		cmd_error(cmd);
		g_status = 127;
	}
	free_str_splited(path);
	return (0);
}
