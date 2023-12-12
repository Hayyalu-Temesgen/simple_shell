#include "shell.h"

/**
 * get_history - this function will get the history file
 * @info: the parameter
 * Return: will return the allocated string
 */

char *get_history(info_t *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_history - this function will create a file
 * @info: the parameter
 * Return: will return 1 on success
 */

int write_history(info_t *info)
{
	ssize_t fd;
	char *filename = get_history(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - this function will read history from file
 * @info: the parameter
 * Return: the return function
 */

int read_history(info_t *info)
{
	int i, last = 0, linecount = 0;
	size_t fd, rdlen, filesize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		filesize = st.st_size;
	if (filesize < 2)
		return (0);
	buf = malloc(sizeof(char) * (filesize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, filesize);
	buf[filesize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < filesize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		history_list(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);
	new_history(info);
	return (info->histcount);
}

/**
 * history_list - this function adds entry to a history list
 * @info: Structure containing potential arguments
 * @buf: the buffer
 * @linecount: the history linecount
 * Return: will always return 0
 */

int history_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
		add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;
		return (0);
}

/**
 * new_history - this function renumbers the history
 * @info: Structure containing potential argument
 * Return: the new histcount
 */

int new_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}
