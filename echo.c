/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 10:19:33 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/30 10:48:01 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	isexpand(char *cmd, int *i, char **env)
{
	int	checking;

	checking = check_cmdinenv(&cmd[*i + 1], env);
	if (checking)
		*i += checking;
	else
		no_cmdinenv(cmd, i);
}

int	check_expvar(char *cmd, t_env *env)
{
	int		i;

	if (check_dollar_sign(cmd) == -1)
		return (0);
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] != '$')
			printf("%c", cmd[i]);
		if (cmd[i] == '$' && !cmd[i + 1])
			printf("$");
		else if (cmd[i] == '$' && cmd[i + 1] == '?')
		{
			printf("%d", g_status);
			i++;
		}
		else if (cmd[i] == '$')
			isexpand(cmd, &i, env->env);
	}
	return (1);
}

void	print_echo(t_env *env, char ***cmds, int i, int *buffer)
{
	int	newline;
	int	dollar_sign;

	cmds[0][i] = smart_trim(cmds[0][i]);
	dollar_sign = check_expvar(cmds[0][i], env);
	newline = 1;
	check_newline(cmds[0], &newline, buffer, i);
	if (!dollar_sign && cmds[0][i] && newline)
		printf("%s", cmds[0][i]);
	if (cmds[0][i] && cmds[0][i + 1] != NULL && newline)
		printf(" ");
}

int	handle_echo(char **cmds, t_env *env)
{
	int	isnull;
	int	buffer;
	int	i;

	isnull = 0;
	if (cmds[1] == NULL)
		isnull = 1;
	i = 0;
	buffer = 0;
	while (!isnull && cmds[++i])
		print_echo(env, &cmds, i, &buffer);
	if (!buffer || isnull)
		printf("\n");
	g_status = 0;
	return (1);
}
