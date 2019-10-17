#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "filetree.h"


void usage(char* name)
{
    printf("Usage: %s [path]\n", name);
    printf("List all files in the path\n");
}

int findFiles(const char* path, FileNode* node)
{
    int c = 0;
    char dirname[256];
    DIR *d;
    struct dirent *att;
    d = opendir(path);
    if (d) {
        while ((att = readdir(d)) != NULL) {
            if (strcmp(att->d_name, ".") == 0) continue;
            if (strcmp(att->d_name, "..") == 0) continue;
            c++;
            FileNode* f1  = node->appendChild(new FileNode(att->d_name, 0, 0,att->d_reclen));
            if (att->d_type == DT_DIR ){
                sprintf(dirname, "%s/%s", path, att->d_name);
                c+=findFiles(dirname, f1);
            }
        }
        closedir(d);
    }

    return c;

}
int main(int argc, char *argv[])
{

    FileNode* fn = new FileNode((const char*) "root", 0, 0,0);
    FileTree* t = new FileTree(fn);
    int nFile = 0;
    char default[] = ".";
    if(argc > 1])
        nFile += findFiles(argv[1], fn);
    else    
        nFile += findFiles(".", fn);

    
    t->print(1);

    delete t; 

    return 0;
}
