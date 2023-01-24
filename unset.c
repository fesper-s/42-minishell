/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:25:22 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/20 22:36:21 by fesper-s         ###   ########.fr       */
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
	buffer[j + 1] = 0;
}

int	handle_unset(char **cmds, t_env **env)
{
	int		i;
	int		j;
	char	**buffer;

	i = 0;
	while (cmds[1] && cmds[++i])
	{
		buffer = malloc(sizeof(char *) * cmds_count((*env)->env));
		attr_buffer(&buffer, cmds[i], (*env)->env);
		free((*env)->env);
		(*env)->env = malloc(sizeof(char *) * (cmds_count(buffer) + 1));
		j = -1;
		while (buffer[++j])
			(*env)->env[j] = ft_strdup(buffer[j]);
		(*env)->env[j] = 0;
		free_charpp(buffer);
	}
	g_status = 0;
	return (1);
}