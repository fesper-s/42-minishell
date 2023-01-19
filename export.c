/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 08:00:46 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/19 10:41:20 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_to_env(char *cmd, t_env **env)
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
	free((*env)->env);
	(*env)->env = malloc(sizeof(char *) * (i + 2));
	i = -1;
	while (buffer[++i])
		(*env)->env[i] = ft_strdup(buffer[i]);
	(*env)->env[i] = 0;
	free_charpp(buffer);
}

int	exporting(char *cmd, t_env **env)
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
	while (cmd[len] != '=')
		++len;
	return (len);
}

void	chenvvar(char *cmd, t_env **env, int i)
{
	free((*env)->env[i]);
	(*env)->env[i] = malloc(sizeof(char) * ft_strlen(cmd) + 1);
	(*env)->env[i] = ft_strdup(cmd);
}

int	handle_export(char **cmds, t_env **env)
{
	int	changed;
	int	i;
	int	j;
	int	len;

	i = 0;
	while (cmds[1] && cmds[++i])
	{
		changed = 0;
		j = -1;
		while ((*env)->env[++j])
		{
			len = count_cmdlen(cmds[i]);
			if (!ft_strncmp(cmds[i], (*env)->env[j], count_cmdlen(cmds[i])) \
				&& (*env)->env[j][len] == '=')
			{
				chenvvar(cmds[i], env, j);
				changed = 1;
			}
		}
		if (!changed)
			exporting(cmds[i], env);
	}
	return (1);
}
