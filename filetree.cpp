#include "filetree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
//////////////////////////////////////////////////////////////////////////////
// File utilities

int stateFile(const char* path, bool* isDir, long* lastTime, long* fileSize)
{
    int ret = 0;
    struct stat buf;
    ret = stat(path, &buf);
    if (ret != 0) {
fprintf(stderr, "state file %s error %d!\n", path, ret);
        return ret;
    }
    *isDir = ((buf.st_mode & S_IFDIR) != 0);
    *lastTime = buf.st_mtime;
    *fileSize = buf.st_size;
    return 0;
}
int findFiles(const char* path, FileNode* node)
{
    int c = 0;
    bool isDir;
    long tm;
    long size;
    char pathname[512];
    DIR *d;
    struct dirent *att;
    d = opendir(path);
    if (d) {
        while ((att = readdir(d)) != NULL) {
            if (strcmp(att->d_name, ".") == 0) continue;
            if (strcmp(att->d_name, "..") == 0) continue;
            sprintf(pathname, "%s/%s", path, att->d_name);

            if( 0!= stateFile(pathname, &isDir, &tm, &size))
                continue;

            c++;

            FileNode* f1  = node->appendChild(new FileNode(pathname, isDir, tm, size));

            if (f1->isFolder() ){

                c+=findFiles(pathname, f1);
            }
        }
        closedir(d);
    }

    return c;

}
//////////////////////////////////////////////////////////////////////////////
FileNode::FileNode(const char* name, bool  isFolder, long time, long size):
        Tree()
{
    if(name)
        mName = strdup(name);
    else
        mName = strdup("");

    mTime = time;
    mSize = size;
    mIsFolder = isFolder;
        
}


FileNode::~FileNode()
{
    if (mName) 
        free(mName);
    

}
const char* FileNode::getFilePathName()
{
    return mName;
}
const char* FileNode::getFileName()
{
    if (!mName)
        return NULL;
    char* p1 = strrchr(mName, '/');
    if(!p1) return mName;
    return p1+ 1;
}
void FileNode::print(int opt)
{
    for (int i=0; i< mDepth; i++)
        printf(" ");

    if (opt) //long display        
    {
        struct tm* tc = localtime((time_t*)&mTime);

        if(!mIsFolder){
            printf("  %s\t %4d/%02d/%02d %02d:%02d:%02d %ldB\n", mName, tc->tm_year+1900, tc->tm_mon+1, tc->tm_mday,
                    tc->tm_hour, tc->tm_min, tc->tm_sec, mSize); 
        }
        else {
            printf("+ %s\t  %4d/%02d/%02d %02d:%02d:%02d\n", mName, tc->tm_year+1900, tc->tm_mon+1, tc->tm_mday,
                    tc->tm_hour, tc->tm_min, tc->tm_sec);
        }
    }
    else
    {
        if(!mIsFolder) {
            printf("  %s\n", mName);
        }
        else
            printf("+ %s\n", mName);
    }
    FileNode* fd = (FileNode*) mFirst;
    while (fd){
        fd->print(opt);
        fd =(FileNode*)fd->mNext;
    }
}

bool FileNode::isFolder() {
    return (mIsFolder);
}


/* append a node after current child */
FileNode* FileNode::appendChild(FileNode* node)
{
    return (FileNode*) Tree::appendChild( node);
}
  
FileNode* FileNode::search(const char* name)
{
    return NULL;
}

