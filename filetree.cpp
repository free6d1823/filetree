#include "filetree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

FileNode::FileNode(const char* name, int isFile, long time, long size):
        mParent(0),mPrev(0), mNext(0), mFirst(0), mLast(0)
{
    if(name)
        mName = strdup(name);
    else
        mName = strdup("");
    mTime = time;
    mSize = size;
    mIsFile = isFile;
    mDepth = 0;

}

FileNode::~FileNode()
{
    if (mName) 
        free(mName);
}
void FileNode::print(int opt)
{
    for (int i=0; i< mDepth; i++)
        printf("  ");
    if (opt) //long display        
        printf("%s %ld %ldKB\n", mName, mTime, mSize); 
    else
        printf("%s\n", mName);
    FileNode* fd = mFirst;
    while (fd){
        fd->print(opt);
        fd =fd->mNext;
    }
}
int FileNode::getDepth()
{
    return mDepth;
}

void FileNode::setDepth(int depth)
{
    mDepth = depth;
}
    /* append a node after current child */
FileNode* FileNode::appendChild(FileNode* node)
{
    if (NULL == node || node->mParent || node->mPrev || node->mNext)
        return NULL;
    node->mParent = this;
    node->mPrev = mLast;
    node->mNext = NULL;    

    if(mLast == NULL) {
        mFirst = node;
    } else {
        mLast->mNext = node;
    }
    mLast = node; 
    node->mDepth = mDepth +1;

    return node;
}
FileNode* FileNode::appendChild(const char* name, int isFile, long time, long size)
{
    return appendChild( new FileNode(name, isFile, time, size));
}

int FileNode::children()
{
    int c = 0;
    FileNode* nd = mFirst;
    while (nd){
        c++;
        nd = nd->mNext;
    }
    return c;
}

///////////////////////////////////////////////////

FileTree::FileTree(FileNode* root)
{
    mRoot = root;
    mRoot->mParent = nullptr;
}
FileTree::~FileTree(){
   if (mRoot)
       delete mRoot;
}
FileNode* FileTree::getRoot()
{
    return mRoot;
}
void FileTree::print(int opt){
        if(mRoot)
            mRoot->print(opt);
        printf("\n");
}

