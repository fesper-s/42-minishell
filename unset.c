/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:25:22 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/20 17:23:00 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_unset(char **cmds, t_env **env)
{
	int		i;
	int		j;
	int		k;
	char	**buffer;

	i = 0;
	while (cmds[1] && cmds[++i])
	{
		buffer = malloc(sizeof(char *) * cmds_count((*env)->env));
		j = 0;
		k = -1;
		while ((*env)->env[j])
		{
			if (!ft_strncmp(cmds[i], (*env)->env[j], ft_strlen(cmds[i])) \
				&& (*env)->env[j][ft_strlen(cmds[i])] == '=')
				j++;
			else
			{
				buffer[++k] = ft_strdup((*env)->env[j]);
				j++;
			}
		}
		buffer[k + 1] = 0;
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
