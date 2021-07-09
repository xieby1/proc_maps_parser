#include "../pmparser.h"

/**
 *
 * Usage: map [pid]
 */

int main(int argc, char *argv[])
{
    char *path;
    if (argc == 2)
        path = argv[1];
    else
        path = "/proc/self/maps";

    procmaps_iterator *maps = pmparser_parse_file(path);
    if (maps == NULL)
    {
        printf("[map]: cannot parse the memory map of %s\n", path);
        return -1;
    }

    // iterate over areas
    for (procmaps_struct *map = pmparser_head(maps); map != NULL;
         map = pmparser_next(maps))
    {
        pmparser_print(map, 0);
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }

    // mandatory: should free the list
    pmparser_free(maps);

    return 0;
}
