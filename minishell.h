/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:49 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/07 08:52:00 by gussoare         ###   ########.fr       */
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
	pid_t			child;
	char			**cmds;
	char			*infile;
	char			*outfile;
	int				infile_id;
	int				outfile_id;
	int				extract_op;
	char			*insert_op;
	char			**env;
	struct s_line	*next;
}					t_line;

typedef struct s_env
{
	char	**env;
}			t_env;

extern int	g_status;

// minishell.c
void	open_files(t_line **line);
int		expand_var(t_line **line, t_env *env);
void	insert_exec(t_line **line);
void	exec_cmds(t_line **line, char *path, int *fd, int *fdd);
void	pipeline(t_line **line, int size);
void	cmd_process(t_line **line, t_env **env);
void	minishell(char **envp);
//check.c
int		check_operator(t_line **line, char **cmds);
int		check_quote_on(char *cmd);
int		organize_line(t_line **line);
int		check_double_pipes(t_line *line);
char	*check_space(char *cmd);
char	*put_space(char *cmd, int x);
void	check_for_pipes(t_line **line, char **cmds);
//init_struct.c
int		file_len(char **cmd);
void	ch_cmd_data(t_line **line, char ***buffer, int j);
int		init_files(t_line **line);
int		init_cmds(t_line **line, char **split);
void	init_linked_list(t_line **line, char **before_pipe, char **after_pipe);
// error.c
int		print_error(char *str);
void	cmd_error(char *cmd);
void	error_display(char *cmd);
void	dir_error(char *path);
int		export_error(char *str);
// more_error.c
void	path_error(char **path, char *cmd);
void	check_dir_error(char *str);
// signal.c
void	signals(void);
void	handle_sigint(int signum);
// utils.c
char	*check_for_path(char **env, char *env_path);
char	*check_cmdpath(char *env_path, char **path, char *cmd);
char	*find_path(t_line **line);
int		cmds_count(char **split);
// builtins_utils.c
int		is_flag(char **cmds, int i);
void	check_newline(char **cmds, int *newline, int *buffer, int i);
void	expanding(t_line **line, t_env *env);
int		cmds_til_pipe(char **cmds);
int		check_dir(char **cmds, char **env);
// list_utils.c
void	lst_free(t_line **lst);
void	ft_lst_add_back(t_line **lst, t_line *new);
t_line	*ft_lst_new(char **cmds, char *infile, char *outfile);
int		ft_lst_size(t_line *lst);
t_line	*ft_lst_last(t_line *lst);
// builtins.c
char	*smart_trim(char *cmd);
int		handle_cd(char **cmds, t_line **env);
int		handle_pwd(t_line *env);
int		handle_env(t_line *env);
int		handle_builtins(char **cmds, t_line **env);
// echo.c
void	isexpand(char *cmd, int *i, char **env);
int		check_expvar(char *cmds, t_line *env);
void	print_echo(t_line *env, char ***cmds, int i, int *buffer);
int		handle_echo(char **cmds, t_line *env);
// echo-utils.c
int		check_dollar_sign(char *cmd);
int		check_cmdinenv(char *cmd, char **env);
void	no_cmdinenv(char *cmd, int *i);
// chdir.c
void	return_dir(t_line **env, int j);
void	relative_path(char *cmd, t_line **env, int j);
void	chpwd(char *cmd, t_line **env, int j);
char	*tilde_home(char *cmd, char *home);
char	*no_argincd(char **env);
// export.c
void	add_to_env(char *cmd, t_line **env);
int		exporting(char *cmd, t_line **env);
void	chenvvar(char *cmd, t_line **env, int i);
int		count_cmdlen(char *cmd);
int		handle_export(char **cmds, t_line **env);
// unset.c
void	attr_buffer(char ***buffer, char *cmd, char **env);
int		check_cmd_env(char **env, char *cmd);
int		handle_unset(char **cmds, t_line **env);
// memory.c
char	**ft_strdupp(char **str);
int		free_charpp(char **str);
void	exiting(t_line **line, t_env **env);

#endif
