/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:25:22 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/07 13:29:52 by gussoare         ###   ########.fr       */
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

int	handle_unset(char **cmds, t_env **env)
{
	int		i;
	char	**buffer;

	i = 0;
	while (cmds[1] && cmds[++i])
	{
		if (check_cmd_env((*env)->env, cmds[i]))
		{
			buffer = malloc(sizeof(char *) * cmds_count((*env)->env));
			attr_buffer(&buffer, cmds[i], (*env)->env);
			free_charpp((*env)->env);
			(*env)->env = ft_strdupp(buffer);
			free_charpp(buffer);
		}
	}
	g_status = 0;
	return (1);
}
