/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 08:00:46 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/07 10:58:31 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_to_env(char *cmd, t_line **env)
{
	int		i;
	char	**buffer;

	i = cmds_count((*env)->env);
	buffer = malloc(sizeof(char *) * (i + 2));
	i = -1;
	while ((*env)->env[++i])
		buffer[i] = ft_strdup((*env)->env[i]);
	buffer[i] = ft_strdup(cmd);
	buffer[i + 1] = 0;
	free_charpp((*env)->env);
	(*env)->env = malloc(sizeof(char *) * (i + 2));
	i = -1;
	while (buffer[++i])
		(*env)->env[i] = ft_strdup(buffer[i]);
	(*env)->env[i] = 0;
	free_charpp(buffer);
}

int	exporting(char *cmd, t_line **env)
{
	int	i;
	int	isattr;

	isattr = 0;
	if (ft_isalpha(cmd[0]) || cmd[0] == '_')
	{
		i = 0;
		while (cmd[++i])
		{
			if (cmd[i] == '=')
			{
				isattr = 1;
				break ;
			}
			if (!ft_isalnum(cmd[i]))
				return (export_error(cmd));
		}
		if (isattr)
			add_to_env(cmd, env);
		g_status = 0;
	}
	else
		export_error(cmd);
	return (0);
}

int	count_cmdlen(char *cmd)
{
	int	len;

	len = 0;
	while (cmd[len] && cmd[len] != '=')
		len++;
	return (len);
}

int	handle_export(char **cmds, t_line **env)
{
	int	changed;
	int	i;
	int	j;

	i = 0;
	while (cmds[1] && cmds[++i])
	{
		changed = 0;
		j = -1;
		while ((*env)->env[++j])
		{
			if (!ft_strncmp(cmds[i], (*env)->env[j], count_cmdlen(cmds[i])) \
				&& (*env)->env[j][count_cmdlen(cmds[i])] == '=')
			{
				free((*env)->env[j]);
				(*env)->env[j] = ft_strdup(cmds[i]);
				changed = 1;
			}
		}
		if (!changed && ft_strchr(cmds[i], '='))
			exporting(cmds[i], env);
	}
	return (1);
}
