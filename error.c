/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 14:52:21 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/13 12:36:21 by gussoare         ###   ########.fr       */
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

void	dir_error(char *path)
{
	ft_putstr_fd("cd: no such file or directory: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd("\n", 2);
}

void	export_error(char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_status = 1;
}
