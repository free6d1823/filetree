#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "filetree.h"


void usage(char* name)
{
    printf("Usage: %s [path]\n", name);
    printf("List all files in the path\n");
}

int main(int argc, char *argv[])
{
    const char* pFolder = (argc > 1)?argv[1]:".";
    bool isDir;
    long tm;
    long size;

    int ret = stateFile(pFolder, &isDir, &tm, &size);
    if (ret != 0) {
        fprintf(stderr, "wrong folder name %s\n", pFolder);
        return -1;
    }

    FileNode* rootFn = new FileNode(pFolder, isDir, tm, size);

    int nFile = findFiles(pFolder, rootFn);

    rootFn->print(1);
    printf("Total %d files\n", nFile);


    delete rootFn; 

    return 0;
}
