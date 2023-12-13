#include "shell.h"

/**
 * my_env - this function will print the current environment
 * @info: Structure containing potential arguments
 * Return: will always return 0
 */

int my_env(info_t *info)
{
	print_list_str(info->env);
		return (0);
}

/**
 * get_env - this function get the value of an environ variable
 * @info: Structure containing potential arguments
 * @name: the name of env
 * Return: will return the value of env
 */

char *get_env(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * my_set_env - this function will initialize a new environment variable
 * @info: Structure containing potential arguments
 * Return: will always return 0
 */

int my_set_env(info_t *info)
{
	if (info->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * my_unset_env - this function will remove an environment variable
 * @info: Structure containing potential arguments
 * Return: will always return 0
 */

int my_unset_env(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unsetenv(info, info->argv[i]);
	return (0);
}

/**
 * pop_env_list - this function will populate env linked list
 * @info: Structure containing potential arguments
 * Return: will always return 0
 */

int pop_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}

