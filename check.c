/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 14:42:32 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/05 14:45:01 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	put_space(t_line *line, int x)
{
	int		i;
	int		j;
	int		len;
	char	*new_cmd;

	i = 0;
	j = 0;
	len = ft_strlen(line->cmd);
	new_cmd = malloc((len + 2) * sizeof(char));
	while (line->cmd[i])
	{
		if (x == j)
		{
			new_cmd[j++] = ' ';
		}
		else
		{
			new_cmd[j] = line->cmd[i++];
			j++;
		}
	}
	new_cmd[j] = 0;
	free(line->cmd);
	line->cmd = ft_strdup(new_cmd);
	free(new_cmd);
}

int	check_space(t_line *line)
{
	int	i;

	i = 0;
	while (line->cmd[i])
	{
		if (line->cmd[i] == '<' && (line->cmd[i + 1] != ' ' \
				&& line->cmd[i + 1] != '<'))
			put_space(line, i + 1);
		else if ((line->cmd[i] != ' ' && line->cmd[i] != '>' \
				&& line->cmd[i + 1] == '>'))
			put_space(line, i + 1);
		else if (line->cmd[i] == '>' && (line->cmd[i + 1] != ' ' \
				&& line->cmd[i + 1] != '>' && line->cmd[i + 1] != 0))
			put_space(line, i + 1);
		else if ((line->cmd[i] == '|' && line->cmd[i + 1] != ' ') \
				|| (line->cmd[i] != ' ' && line->cmd[i + 1] == '|'))
			put_space(line, i + 1);
		else
			i++;
	}
	return (1);
}

void	check_line(t_line *line)
{
	int	i;

	i = -1;
	while (line->cmd[++i])
	{
		if (line->cmd[i] == '|' && line->cmd[i + 1] == '|')
		{
			ft_putstr_fd("Error: Can't use || or &&\n", 2);
			exit(EXIT_FAILURE);
		}
		if (line->cmd[i] == '&' && line->cmd[i + 1] == '&')
		{
			ft_putstr_fd("Error: Can't use || or &&\n", 2);
			exit(EXIT_FAILURE);
		}
	}
}

int organize_line(t_line *line)
{
	int		i;
	char	**split_line;

	i = -1;
	if (!line->cmd)
		return (0);
	check_line(line);
	check_space(line);
	split_line = ft_split(line->cmd, ' ');
	while (split_line[++i])
		printf("cmd[%d]--> %s\n", i, split_line[i]);

	return (1);
}