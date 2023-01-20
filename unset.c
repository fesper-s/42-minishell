/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:25:22 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/20 15:44:48 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checkvartodel(char *cmd, char **env)
{
	int	i;
	int	found;

	i = -1;
	found = 0;
	while (env[++i])
	{
		if (!ft_strncmp(cmd, env[i], ft_strlen(cmd)) && \
			env[i][ft_strlen(cmd)] == '=')
		{
			found = 1;
			break ;
		}
	}
	if (!found)
		return (0);
	return (i);
}

int	handle_unset(char **cmds, t_env **env)
{
	int		i;
	int		j;
	int		vartodel;
	char	**buffer;

	if (!cmds[1])
		return (1);
	vartodel = checkvartodel(cmds[1], (*env)->env);
	if (vartodel && !ft_strncmp((*env)->env[vartodel], \
		cmds[1], ft_strlen(cmds[1])) && \
		(*env)->env[vartodel][ft_strlen(cmds[1])] == '=')
	{
		buffer = malloc(sizeof(char *) * cmds_count((*env)->env));
		i = -1;
		j = 0;
		while ((*env)->env[++i])
		{
			if ((*env)->env[j] && !ft_strncmp((*env)->env[vartodel], \
				(*env)->env[i], ft_strlen((*env)->env[vartodel])) && \
				(*env)->env[vartodel][ft_strlen(cmds[1])] == '=')
			{
				//printf("%s\n", (*env)->env[j]);
				j++;
			}
			else if ((*env)->env[j])
			{
				//printf("%s\n", (*env)->env[j]);
				buffer[i] = ft_strdup((*env)->env[j]);
				j++;
			}
		}
		buffer[i] = 0;
		free((*env)->env);
		(*env)->env = malloc(sizeof(char *) * cmds_count(buffer));
		i = -1;
		while (buffer[++i])
			(*env)->env[i] = ft_strdup(buffer[i]);
		(*env)->env[i] = 0;
		free_charpp(buffer);
	}
	return (1);
}
