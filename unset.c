/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:25:22 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/14 12:12:45 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	attr_buffer(char ***buffer, char *cmd, char **env)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	while (env[i])
	{
		if (!ft_strncmp(cmd, env[i], ft_strlen(cmd)) \
			&& env[i][ft_strlen(cmd)] == '=')
			i++;
		else
		{
			buffer[0][++j] = ft_strdup(env[i]);
			i++;
		}
	}
	buffer[0][j + 1] = 0;
}

int	check_cmd_env(char **env, char *cmd)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(cmd, env[i], ft_strlen(cmd)) && \
			env[i][ft_strlen(cmd) == '='])
			return (1);
	}
	return (0);
}

int	validate_for_unset(char *cmds)
{
	int	i;

	i = -1;
	while (cmds[++i])
	{
		if (ft_isdigit(cmds[i]) && i == 0)
		{
			export_error(cmds, "unset");
			return (1);
		}
		if (!ft_isalnum(cmds[i]) && cmds[i] != '_')
		{
			export_error(cmds, "unset");
			return (1);
		}
	}
	return (0);
}

int	handle_unset(char **cmds, t_env **env)
{
	int		i;
	char	**buffer;

	g_status = 0;
	i = 0;
	while (cmds[1] && cmds[++i])
	{
		if (validate_for_unset(cmds[i]))
			break ;
		if (check_cmd_env((*env)->env, cmds[i]))
		{
			buffer = malloc(sizeof(char *) * cmds_count((*env)->env));
			attr_buffer(&buffer, cmds[i], (*env)->env);
			free_charpp((*env)->env);
			(*env)->env = ft_strdupp(buffer);
			free_charpp(buffer);
		}
	}
	return (1);
}
