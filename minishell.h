/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:49 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/11 07:44:14 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <sys/wait.h>

typedef struct s_line
{
	char			*cmd;
	char			**cmds;
	char			*infile;
	char			*outfile;
	struct s_line	*next;
}					t_line;

typedef struct s_env
{
	char	**env;
}			t_env;

int	g_status;

// minishell.c
void	expand_var(char *cmd);
void	cmd_process(char *cmd, t_env **env);
char	**get_env(char **envp);
void	minishell(char **envp);
// checker
void	check_line(t_line *line);
int		check_space(t_line *line);
void	put_space(t_line *line, int x);
int		organize_line(t_line *line);
//init
void	init_values(t_line *line);
void	init_files(t_line *line, char **split);
void	init_cmds(t_line *line, char **split);
// error.c
int		print_error(char *str);
void	cmd_error(char *cmd);
void	error_display(char *cmd);
char	*path_error(char *cmd, char **path);
void	dir_error(char *path);
// signal.c
void	signals(void);
void	handle_sigint(int signum);
// utils.c
int		free_str_splited(char **str);
char	*find_path(char *cmd);
char	**ft_trim(char **cmds);
char	**get_cmds(char *cmd);
size_t	cmds_count(char **split);
// builtins.c
int		handle_pwd_and_env(char **cmds, t_env *env);
int		handle_exports(char **cmds, t_env **env);
int		handle_builtins(char **cmds, t_env **env);
// builtins2.c
void	relative_path(char *cmd, t_env **env, char *pwd, int j);
void	chpwd(char *cmd, t_env **env, int j);
int		handle_cd(char **cmds, t_env **env);

#endif
