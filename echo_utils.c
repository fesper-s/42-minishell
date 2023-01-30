/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 10:24:37 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/30 10:27:01 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_dollar_sign(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
			return (i);
	}
	return (-1);
}

int	check_cmdinenv(char *cmd, char **env)
{
	int		i;
	int		len;

	if (check_dollar_sign(cmd) != -1)
		len = check_dollar_sign(cmd);
	else
		len = ft_strlen(cmd);
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], cmd, len) && env[i][len] == '=')
		{
			printf("%s", &env[i][len + 1]);
			return (count_cmdlen(env[i]));
		}
	}
	return (0);
}

void	no_cmdinenv(char *cmd, int *i)
{
	*i += 1;
	while (cmd[*i])
	{
		if (ft_isalnum(cmd[*i]) || cmd[*i] == '_')
			*i += 1;
		else
		{
			*i -= 1;
			break ;
		}
	}
}
