/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 11:26:57 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/24 08:27:16 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	path_error(char **path, char *cmd)
{	
	if (path)
		error_display(cmd);
	else
	{
		print_error("minishell: ");
		print_error(cmd);
		print_error(": No such file or directory\n");
		g_status = 127;
	}
}

void	check_dir_error(char *str)
{
	print_error("minishell: ");
	print_error(str);
	print_error(": is a directory\n");
	g_status = 126;
}
