/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 14:52:21 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/14 12:13:57 by fesper-s         ###   ########.fr       */
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
	print_error("minishell: ");
	print_error(cmd);
	print_error(": command not found\n");
}

void	error_display(char *cmd)
{
	cmd_error(cmd);
	g_status = 127;
}

void	dir_error(char *path)
{
	ft_putstr_fd("minishell: cd: no such file or directory: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd("\n", 2);
	g_status = 1;
}

int	export_error(char *str, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_status = 1;
	return (1);
}
