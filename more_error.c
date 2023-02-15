/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 11:26:57 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/15 08:07:23 by fesper-s         ###   ########.fr       */
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

char	*checking_exe(t_line **line, char **env_path)
{
	g_status = 126;
	if (!ft_strncmp((*line)->cmds[0], "/bin/", 5) || \
		!ft_strncmp((*line)->cmds[0], "/usr/bin/", 9) || \
		!ft_strncmp((*line)->cmds[0], "./", 2))
		g_status = 0;
	free(env_path[0]);
	return (ft_strdup((*line)->cmds[0]));
}
