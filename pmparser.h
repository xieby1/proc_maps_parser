/*
 @Author	: ouadimjamal@gmail.com
 @date		: December 2015

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.  No representations are made about the suitability of this
software for any purpose.  It is provided "as is" without express or
implied warranty.

 */

#ifndef H_PMPARSER
#define H_PMPARSER
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// maximum line length in a procmaps file
#define PROCMAPS_LINE_MAX_LENGTH (PATH_MAX + 100)
/**
 * procmaps_struct
 * @desc hold all the information about an area in the process's  VM
 */
typedef struct procmaps_struct
{
    void *addr_start;     //< start address of the area
    void *addr_end;       //< end address
    unsigned long length; //< size of the range

    char perm[5]; //< permissions rwxp
#define PERM_R 8
#define PERM_W 4
#define PERM_X 2
#define PERM_P 1
    char rwxp;

    long offset;  //< offset
    char dev[12]; //< dev major:minor
    int inode;    //< inode of the file that backs the area

    char pathname[600]; //< the path of the file that backs the area
    // chained list
    struct procmaps_struct *next; //<handler of the chinaed list
} procmaps_struct;

/**
 * procmaps_iterator
 * @desc holds iterating information
 */
typedef struct procmaps_iterator
{
    procmaps_struct *head;
    procmaps_struct *current;
} procmaps_iterator;
/**
 * pmparser_parse
 * @param pid the process id whose memory map to be parser. the current process
 * if pid<0
 * @return an iterator over all the nodes
 */
procmaps_iterator *pmparser_parse(int pid);

/**
 * pmparser_parse_file
 * @desc same behavior with pmparser_parse, except input is maps_path
 * @param maps_path the path to maps file
 * @return see return of procmaps_iterator
 */
procmaps_iterator *pmparser_parse_file(char *maps_path);

/**
 * @brief Reset iterator and get head element
 * @param p_procmaps_it The iterator
 * @return The head element of iterator
 */
procmaps_struct *pmparser_head(procmaps_iterator *p_procmaps_it);

/**
 * pmparser_next
 * @description move between areas
 * @param p_procmaps_it the iterator to move on step in the chained list
 * @return a procmaps structure filled with information about this VM area
 */
procmaps_struct *pmparser_next(procmaps_iterator *p_procmaps_it);
/**
 * pmparser_free
 * @description should be called at the end to free the resources
 * @param p_procmaps_it the iterator structure returned by pmparser_parse
 */
void pmparser_free(procmaps_iterator *p_procmaps_it);

/**
 * _pmparser_split_line
 * @description internal usage
 */
void _pmparser_split_line(char *buf, char *addr1, char *addr2, char *perm,
                          char *offset, char *device, char *inode,
                          char *pathname);

/**
 * pmparser_print
 * @param map the head of the list
 * @order the order of the area to print, -1 to print everything
 */
void pmparser_print(procmaps_struct *map, int order);

#endif
