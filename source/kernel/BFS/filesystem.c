#include "filesystem.h"

#include "../memory/heap.h"
#include "../memory/memory.h"

/*
 * My 'Temp File System' implementation (BFS - Butterfly File System).
 *
 * It's like a file system, but with the twist that it lives in your computer's RAM, so it's
 * super fast and disappears when you turn off your computer—like magic! Implemented because
 * initrd was kinda meh and FAT16 and ext2 were... well, let's just say they were a bit too
 * much drama for this project. So here we are, making our own file system, because why not?
*/

/* from console.c we import ... */

extern void ttyPrintStr(const char *string);
extern void ttyPrintFmt(const char *format, ...);


/** The File system's root directory */
Directory *BFS_PRIMARY_DIR;

/** The File system's currrent directory */
Directory *BFS_CURRENT_DIR;


void mountFileSystem(void) {
    BFS_PRIMARY_DIR = (Directory *) memoryAllocateBlock(sizeof(Directory));
    BFS_PRIMARY_DIR->name = (char *) memoryAllocateBlock(MAX_NAME_LEN);
    strcpy(BFS_PRIMARY_DIR->name, "/");

    BFS_PRIMARY_DIR->parent = NULL;
    BFS_PRIMARY_DIR->subdirs = NULL;
    BFS_PRIMARY_DIR->next = NULL;
    BFS_PRIMARY_DIR->files = NULL;

    BFS_CURRENT_DIR = BFS_PRIMARY_DIR;
}


bool bfsCheckName(const char *name) {
    if ((*name == '\0') || (strcmp(name, " ") == 0)) {
        return false; // If the name is empty or is only an space
    }

    const char *invalidChars = "\\/:;*?\"<>|-";
    while (*name) {
        if (strchr(invalidChars, *name)) {
            return false; // Invalid char found
        }
        name++;
    }
    return true;
}


/* If you think this is complete spaghetti, I recommend you see the vprintFormat() code, LOL */
static void bfsCreateTree(Directory *directory, uint8_t level, bool *branch_flags) {
    // Print the leading characters for the current level's tree structure
    for (uint8_t i = 0; i < (level - 1); ++i) {
        if (branch_flags[i]) {
            // Print vertical line if there is a branch at this level
            ttyPrintStr("|    ");
        } else {
            // Print spaces if there is no branch
            ttyPrintStr("     ");
        }
    }

    // Print the directory name with a vertical line or '+' based on the level
    if (level > 0) {
        ttyPrintStr("|\n"); // Print vertical line before directory name
        for (uint8_t i = 0; i < (level - 1); ++i) {
            if (branch_flags[i]) {
                // Print vertical line if there is a branch
                ttyPrintStr("|    ");
            } else {
                // Print spaces if there is no branch
                ttyPrintStr("     ");
            }
        }

        // Print directory name with tree indicator
        ttyPrintFmt("+---[%s]\n", directory->name);
    } else {
        // Print root directory name without vertical line
        ttyPrintFmt("+---[%s]\n", directory->name);
    }

    /// Print all files in the current directory
    File *file = directory->files;
    while (file) {
        // Print leading characters for each file based on the level
        for (uint8_t i = 0; i < level; ++i) {
            if (branch_flags[i]) {
                ttyPrintStr("|    "); // Print vertical line if there is a branch
            } else {
                ttyPrintStr("     "); // Print spaces if there is no branch
            }
        }

        // Print vertical line before file name
        ttyPrintStr("|\n");

        for (uint8_t i = 0; i < level; ++i) {
            if (branch_flags[i]) {
                // Print vertical line if there is a branch
                ttyPrintStr("|    ");
            } else {
                // Print spaces if there is no branch
                ttyPrintStr("     ");
            }
        }

        // Print the file name
        ttyPrintFmt("+--->%s\n", file->name, file->size);
        file = file->next;  // Move to the next file in the list
    }

    /// Recurse into each subdirectory
    Directory *subdir = directory->subdirs;
    while (subdir) {
        // Set the branch flag for the current level
        if (subdir->next) {
            // There are more directories at this level
            branch_flags[level] = true;
        } else {
            // No more directories at this level
            branch_flags[level] = false;
        }

        // Recursively print subdirectories
        bfsCreateTree(subdir, (level + 1), branch_flags);
        subdir = subdir->next; // Move to the next subdirectory
    }
}


File *bfsCreateFile(Directory *parent, const char *name) {
    File *file = (File *) memoryAllocateBlock(sizeof(File));

    file->name = (char *) memoryAllocateBlock(MAX_NAME_LEN);
    strcpy(file->name, name);

    file->size = 0; // Empty file, lol

    file->next = parent->files;
    parent->files = file;

    return file;
}


Directory *bfsCreateDirectory(Directory *parent, const char *name) {
    Directory *directory = (Directory *) memoryAllocateBlock(sizeof(Directory));

    directory->name = (char *) memoryAllocateBlock(MAX_NAME_LEN);
    strcpy(directory->name, name);

    directory->parent = parent;
    directory->subdirs = NULL;
    directory->next = NULL;
    directory->files = NULL;

    directory->next = parent->subdirs;
    parent->subdirs = directory;

    return directory;
}


Directory *bfsFindDirectory(const char *path) {
    Directory *directory = BFS_PRIMARY_DIR;
    if (path[0] == '/') {
        path++; // Skip the first '/'
    }

    char temp[MAX_NAME_LEN];
    while (*path) {
        const char *slash = strchr(path, '/');
        if (slash) {
            strncpy(temp, path, slash - path + 1);
            temp[slash - path] = '\0';
            path = slash + 1;
        } else {
            strcpy(temp, path);
            path += strlen(path);
        }

        Directory *subdir = directory->subdirs;
        while ((subdir) && (strcmp(subdir->name, temp) != 0)) {
            subdir = subdir->next;
        }

        if (!subdir) {
            return NULL; // Directory not found
        }

        directory = subdir;
    }
    return directory;
}


File *bfsFindFile(const char *path) {
    char *slash = strrchr(path, '/');
    Directory *directory;

    if (slash) {
        char dirPath[MAX_NAME_LEN];
        strncpy(dirPath, path, slash - path + 1);
        dirPath[slash - path] = '\0';
        directory = bfsFindDirectory(dirPath);
        path = slash + 1;
    } else {
        directory = BFS_CURRENT_DIR;
    }

    if (!directory) {
        return NULL; // Directory not found
    }

    File *file = directory->files;
    while ((file) && (strcmp(file->name, path) != 0)) {
        file = file->next;
    }

    return file;
}


void bfsCopyFile(File *file, Directory *destination, const char *newName) {
    if (!file || !destination) return;

    File *newFile = bfsCreateFile(destination, newName ? newName : file->name);
    strncpy(newFile->content, file->content, MAX_CONTENT_LEN);
    newFile->size = file->size;
}


void bfsRemoveFile(Directory *parent, const char *name) {
    File **current = &parent->files;
    while ((*current) && (strcmp((*current)->name, name) != 0)) {
        current = &(*current)->next;
    }

    if (*current) {
        File *temp = *current;
        *current = (*current)->next;
        memoryFreeBlock(temp->name); // Free memory from name
        memoryFreeBlock(temp); // Free mmeory from struct
    }
}

void bfsRemoveDirectory(Directory *directory) {
    if (!directory) return;

    /* We remove any file inside */
    while (directory->files) {
        File *temp = directory->files;
        directory->files = directory->files->next;
        memoryFreeBlock(temp->name); // Free memory from filenames
        memoryFreeBlock(temp); // free memory from files
    }

    /* Same with subdirs */
    while (directory->subdirs) {
        Directory *temp = directory->subdirs;
        directory->subdirs = directory->subdirs->next;
        bfsRemoveDirectory(temp); // free memory from subdirs
    }

    /* Finally we remove the directory */
    memoryFreeBlock(directory->name); // Free memory from name
    memoryFreeBlock(directory); // Free memory from struct
}


void bfsWriteFile(File *file, const char *content) {
    strncpy(file->content, content, MAX_CONTENT_LEN);
    file->size = strlen(content);
}


void bfsReadFile(File *file) {
    ttyPrintFmt("# Content of %s: %s\n", file->name, file->content);
}


void bfsPrintTree(Directory *directory, uint8_t level) {
    // We assume that the maximum depth level for dirs is 64 ...
    bool branch_flags[64] = { false };
    bfsCreateTree(directory, level, branch_flags);
}
