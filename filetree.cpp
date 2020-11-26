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
//expand childrens. don't do twice.
int FileNode::expand()
{
	int c = 0;
    bool isDir;
    long tm;
    long size;
    DIR *d;
    struct dirent *att;
	char* currentFolder = getFilePathName();
	char pathname[512];
	if (currentFolder) {
    	d = opendir(currentFolder);	
		if (d) {
			while ((att = readdir(d)) != NULL) {
				if (strcmp(att->d_name, ".") == 0) continue;
				if (strcmp(att->d_name, "..") == 0) continue;
				sprintf(pathname, "%s/%s", currentFolder, att->d_name);
			
				if( 0!= stateFile(pathname, &isDir, &tm, &size))
					continue;
				FileNode* f1 = new FileNode(att->d_name, isDir, tm, size);
				appendChild(f1);
				c++;
				if (f1->isFolder() ){
                	c+=f1->expand();
				}
            }
			closedir(d);
		}
		free(currentFolder);
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

FileNode* FileNode::CreateRoot(const char* name)
{
	bool isDir;
	long tm;
	long size;

	FileNode* rootFn = NULL;
	int ret = stateFile(name, &isDir, &tm, &size);
	if (ret == 0) {
		rootFn = new FileNode(name, isDir, tm, size);
	}
	return rootFn;
}


FileNode::~FileNode()
{
    if (mName) 
        free(mName);
	if(mFirst) {
		mFirst->remove();
		delete mFirst;
	}
	if(mNext){
		mNext->remove();
		delete mNext;
	}

}
/* free the pointer after used */
char* FileNode::getFilePathName()
{
	Link* pFirst = new Link(this);
	Link* pLast = pFirst;
	int length = strlen(mName) + 1;;
	FileNode* pParent = (FileNode*)mParent;
	while(pParent){
		pLast = pLast->append(pParent);
		length += strlen(pParent->mName) + 1;
	
		pParent = (FileNode*) pParent->mParent;
	}
	
	char* buffer = (char*)malloc(length);
	char* p = buffer;
	Link* lk = pLast;
	while(lk ) {
		FileNode* node = (FileNode*)lk->mData;
		strcpy(p, node->getFileName());
		p += strlen(p);
		if (lk != pFirst)
			*p = '/';
		p++;
		lk = lk->mPrev;
	}

	delete pFirst;
    return buffer;
}
//exclude root folder
char* FileNode::getRelativePathName()
{
	Link* pFirst = new Link(this);
	Link* pLast = pFirst;
	int length = strlen(mName) + 1;;
	FileNode* pParent = (FileNode*)mParent;
	while(pParent){
		if (!pParent->mParent)
			break;
		pLast = pLast->append(pParent);
		length += strlen(pParent->mName) + 1;
	
		pParent = (FileNode*) pParent->mParent;
	}
	if (length == 0) //is root
		return NULL;
	char* buffer = (char*)malloc(length);
	char* p = buffer;
	Link* lk = pLast;
	while(lk ) {
		FileNode* node = (FileNode*)lk->mData;
		strcpy(p, node->getFileName());
		p += strlen(p);
		if (lk != pFirst)
			*p = '/';
		p++;
		lk = lk->mPrev;
	}

	delete pFirst;
    return buffer;
}

const char* FileNode::getFileName()
{
	return mName;
}
void FileNode::print(int opt)
{
    for (int i=0; i< mDepth; i++)
        printf(" ");

    if (opt) //long display        
    {
        struct tm* tc = localtime((time_t*)&mTime);
		char* pName = getRelativePathName();
        if(!mIsFolder){
            printf("  %s\t %4d/%02d/%02d %02d:%02d:%02d %ldB\n", pName, tc->tm_year+1900, tc->tm_mon+1, tc->tm_mday,
                    tc->tm_hour, tc->tm_min, tc->tm_sec, mSize); 
        }
        else {
            printf("+ %s\t  %4d/%02d/%02d %02d:%02d:%02d\n", pName, tc->tm_year+1900, tc->tm_mon+1, tc->tm_mday,
                    tc->tm_hour, tc->tm_min, tc->tm_sec);
        }

		free(pName);
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
	char* pName = getRelativePathName();
	if (strcmp(name, pName) == 0){
		free(pName);
		return this;
	}
	free(pName);
	
	FileNode* fd = (FileNode*) mFirst;
	 while (fd){
        FileNode* fd2 = fd->search(name);
		if (fd2)
			return fd2;
        fd =(FileNode*)fd->mNext;
    }
		
    return NULL;
}
void FileNode::compareAll(FileNode* refRoot, fnComapreCallBack fnCbk)
{
	FileNode* fd = (FileNode*) mFirst;
	 while (fd){
		fd->compare(refRoot, fnCbk);
        fd =(FileNode*)fd->mNext;
    }

}
void FileNode::compare(FileNode* refRoot, fnComapreCallBack fnCbk)
{
	FileNode* node = refRoot->search(getRelativePathName());
	char* p1 = NULL;
	char* p2 = NULL;
	if (!node){
		if(!isFolder()) {
			p1 = getFilePathName();
			fnCbk(p1, NULL, 1, 1); //this only in destination
			free(p1);
		}
		return;
	}

	if ( isFolder())
		return;
	long tmDiff = mTime - node->mTime;
	long sizeDiff = mSize -node->mSize;

	if (tmDiff !=0 || sizeDiff !=0 ) {
		p1 = getFilePathName();
		p2 = node->getFilePathName();
		fnCbk(p1, p2, tmDiff, sizeDiff);
		if (p1) free(p1);
		if (p2) free(p2);
	}

}


