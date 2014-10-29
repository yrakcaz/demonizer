#ifndef MANAGE_H
# define MANAGE_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdio.h>
# include <fcntl.h>
# include <dirent.h>
# include <string.h>

/**
** @def DEAMONS_SIZE
** @brief Maximum size of global array containing deamons indexes.
*/
# define DEAMONS_SIZE 4096

/**
** @var pid_t deamons[]
** @brief Array of DEAMONS_SIZE pids containing deamons' pid at their index.
*/
pid_t deamons[DEAMONS_SIZE];

/**
** @fn char *get_process_cmd(pid_t pid);
** @brief Get the commandline of a process by looking into /proc.
** @param pid Pid of the related process.
** @return Returns the commandline.
*/
char *get_process_cmd(pid_t pid);

/**
** @fn void get_deamons();
** @brief Fills the deamons array.
*/
void get_deamons();

/**
** @fn void display_deamons();
** @brief Displays the deamon list from deamons array.
*/
void display_deamons();


#endif /* !MANAGE_H */
