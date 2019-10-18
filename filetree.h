#ifndef FILE_TREE_H
#define FILE_TREE_H

#include "tree.h"

class FileTree;
class FileNode : public Tree{
    friend FileTree;
public:
    FileNode(const char* name, bool  isFolder=0, long time=0, long size=0);
    ~FileNode();

    void print(int opt);
 
    /* append a node after current child */
    FileNode* appendChild(FileNode* node);
 
    const char* getFilePathName();
    const char* getFileName();
    bool isFolder();
    /* sort childrens as the specified opder */
    //Tree* sortChildren(int option);
    /* search first descendant meets the condition */
    FileNode* search(const char* name);
protected:

private:
    bool  mIsFolder;    
    char* mName;

    long mTime;
    long mSize;
};

int stateFile(const char* path, bool* isDir, long* lastTime, long* fileSize);
int findFiles(const char* path, FileNode* node);
#endif //FILE_TREE_H
