#ifndef FILE_TREE_H
#define FILE_TREE_H

class FileTree;
class FileNode{
    friend FileTree;
public:
    FileNode(const char* name, int isFile=0, long time=0, long size=0);
    ~FileNode();
    void print(int opt);
    int getDepth();
    /* append a node after current child */
    FileNode* appendChild(FileNode* node);
    FileNode* appendChild(const char* name, int isFile=0, long time=0, long size=0);

    /* number of children */
    int children();
protected:
    void setDepth(int depth);
    FileNode* mParent;
    FileNode* mPrev;
    FileNode* mNext;
    FileNode* mFirst;
    FileNode* mLast;
private:
    int  mIsFile;    
    char* mName;
    long mTime;
    long mSize;
    int mDepth;
};

class FileTree {
public:
    FileTree(FileNode* root);
    ~FileTree();
    FileNode* getRoot();
    void print(int opt=0);
private:
    FileNode* mRoot;
};

#endif //FILE_TREE_H
