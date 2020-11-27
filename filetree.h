#ifndef FILE_TREE_H
#define FILE_TREE_H

#include "tree.h"
class FileNode;
typedef void (* fnComapreCallBack)(FileNode* candidate, FileNode* refference);

class FileNode : public Tree{

public:
    FileNode(const char* name, bool  isFolder=0, long time=0, long size=0);
    ~FileNode();
static FileNode* CreateRoot(const char* name);

    void print(int opt);
 
    /* append a node after current child */
    FileNode* appendChild(FileNode* node);
 
    char* createFilePathName();
	char* createRelativePathName();
    const char* getFileName();
    bool isFolder();
	//expand childrens. don't do twice.
	int expand();
    /* sort childrens as the specified opder */
    //Tree* sortChildren(int option);
    /* search first descendant meets the condition */
    FileNode* search(const char* name);
	void compareAll(FileNode* refRoot, fnComapreCallBack fnCbk);
	int compare(FileNode* refRoot, fnComapreCallBack fnCbk);
    long mTime;
    long mSize;	
protected:

private:
    bool  mIsFolder;    
    char* mName;


};

int stateFile(const char* path, bool* isDir, long* lastTime, long* fileSize);
int expandFiles(const char* path, FileNode* node);
#endif //FILE_TREE_H
