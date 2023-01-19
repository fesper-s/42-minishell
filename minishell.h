/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:49 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/19 14:15:36 by fesper-s         ###   ########.fr       */
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
	int				infile_id;
	int				outfile_id;
	struct s_env	*env;
	struct s_line	*next;
}					t_line;

typedef struct s_env
{
	char	**env;
}			t_env;

int	g_status;

// minishell.c
void	expand_var(t_line **line, t_env *env);
void	exec_cmds(t_line **line, pid_t pid, int fdd, int *fd);
void	pipeline(t_line **line, int size);
void	cmd_process(t_line **line, t_env **env);
void	minishell(char **envp);
//check.c
int		organize_line(t_line **line);
void	check_line(t_line *line);
int		check_space(t_line **line);
void	put_space(t_line **line, int x);
void	check_for_pipes(t_line **line, char **cmds);
//init_struct.c
char	**get_env(char **envp);
void	init_files(t_line **line, char **split);
void	init_cmds(t_line **line, char **split);
void	init_linked_list(t_line **line, char **before_pipe, char **after_pipe);
// error.c
int		print_error(char *str);
void	cmd_error(char *cmd);
void	error_display(char *cmd);
void	dir_error(char *path);
int		export_error(char *str);
// signal.c
void	signals(void);
void	handle_sigint(int signum);
// utils.c
int		free_charpp(char **str);
char	*find_path(char *cmd);
char	**ft_trim(char **cmds);
char	**get_cmds(char *cmd);
void	lst_free(t_line **lst);
// builtins_utils.c
int		is_flag(char **cmds, int i);
void	check_newline(char **cmds, int *newline, int *buffer, int i);
int		cmds_count(char **split);
void	expanding(t_line **line, t_env *env);
// list_utils.c
void	ft_lst_add_back(t_line **lst, t_line *new);
t_line	*ft_lst_new(char **cmds, char *infile, char *outfile);
int		ft_lst_size(t_line *lst);
int		cmds_til_pipe(char **cmds);
t_line	*ft_lst_last(t_line *lst);
// builtins.c
int		handle_pwd_and_env(char **cmds, t_env *env);
void	check_expvar(char **cmds, t_env *env, int j);
int		handle_echo(char **cmds, t_env *env);
int		handle_builtins(char **cmds, t_env **env);
// chdir.c
void	relative_path(char *cmd, t_env **env, char *pwd, int j);
void	chpwd(char *cmd, t_env **env, int j);
char	*tilde_home(char *cmd, char *home);
int		handle_cd(char **cmds, t_env **env);
// export.c
void	add_to_env(char *cmd, t_env **env);
int		exporting(char *cmd, t_env **env);
void	chenvvar(char *cmd, t_env **env, int i);
int		count_cmdlen(char *cmd);
int		handle_export(char **cmds, t_env **env);

#endif
