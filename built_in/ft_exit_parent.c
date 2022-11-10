char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (c >= 256)
		c = c % 256;
	while (s[i])
	{
		if (s[i] == c)
			return ((char *)&s[i]);
		i++;
	}
	if (c == '\0')
		return ((char *)&s[i]);
	return (0);
}


int ft_is_exit(char *str)
{
	char	**arg;
	int		i;

	arg = NULL;
	i = -1;
	if ((str == NULL) || (ft_strchr(str, '|') == NULL))
		return (1);
	if ((arg = ft_split(str, ' ')) == NULL)
		return (ft_putstr_fd("Error : split\n", 2), 1);
	while (arg[++i])
	{
		if (ft_strncmp(arg[i], "exit", 4) == 0)
		{
			i = -1;
			while (arg[++i])
				free(arg[i]);
			free(arg);
			return (0);
		}
	}
	i = -1;
	while (arg[++i])
		free(arg[i]);
	free(arg);
	return (1);
}

char	**ft_clear_arg(char **arg, int *exit_status)
{
	char	**new;
	int		i;

	new = NULL;
	i = 0;
	while(arg[i])
	{
		if (ft_strcmp(arg[i], "<") == 0)
		{
			
		}
	}
}

int ft_exit_parent(t_ast *ast, int *exit_status)
{
	long long int	i;
	int				errnum;
	i = 0;
	errnum = 256;

	if (ft_strlen_l(argv) == 1)
		return (0);
	if (ft_only_num(argv[1]) != 0)
		return (ft_exit_error(argv[1], 2), 0);
	while (argv[i])
		i++;
	if (i > 2)
		return (ft_exit_error(NULL, 1), 1);
	i = ft_atolli(argv[1]);
	i = i % 256;
	errnum += i;
	*exit_status = errnum % 256;
	return (0);
}

int	ft_check_exit(char *str, t_env *env, int *exit_status)
{
	t_vec	v;
	t_ast	*ast;
	int		err_code;

	ast = NULL;
	v = lexer(str, *env);
    if (v.len <= 1 || (((t_token *)v.data)[v.len - 1].type) != TOKEN_END)
    {
        vec_free(&v);
        return (1);
    }        
	ast = ast_init(v.data, v.len);
	if (ft_exit_parent(ast, exit_status) != 0)
		return (1);
	ast_free(ast);
	vec_free(&v);
	return (0);
}