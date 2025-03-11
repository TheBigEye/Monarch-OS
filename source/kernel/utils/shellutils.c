#include "shellutils.h"

#include "../BFS/filesystem.h"
#include "../drivers/console.h"

#define ERR_SUCCESS               0x00 // The operation completed successfully.
#define ERR_FILE_NOT_FOUND        0x01 // The system cannot find the file specified.
#define ERR_DIR_NOT_FOUND         0x02 // The system cannot find the path specified.
#define ERR_FILE_EXISTS           0x03 // The specified file already exist.
#define ERR_DIR_EXIST             0x04 // The specified path already exist.
#define ERR_OVERFLOW              0x05 // The filename, path or directory name is too long.

#define ERR_BAD_COMMAND           0xFF //

int shellMakeDirectory(char* input) {
    const char *path = input + 6;
    char *slash = strrchr(path, '/');

    if (slash) {
        char parentPath[MAX_NAME_LEN];
        strncpy(parentPath, path, slash - path + 1);
        parentPath[slash - path] = '\0';
        Directory *parent = bfsFindDirectory(parentPath);
        if (parent) {
            bfsCreateDirectory(parent, slash + 1);
            return 0;
        } else {
            ttyPrintFmt("Parent directory not found\n");
            return 0;
        }
    } else {
        bfsCreateDirectory(BFS_CURRENT_DIR, path);
        return 0;
    }
    return 0;
}
