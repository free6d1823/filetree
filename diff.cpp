#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "filetree.h"


void usage(char* name)
{
    printf("Usage: %s <target_path> [source]\n", name);
    printf("Replace files in <source> with those in <target_path> that is newer.\n");
}

void ComapreCallBack(const char* file, const char* fileRef, long isNewer, long isLarger)
{
	if(fileRef == NULL)
		printf("%s is only in the folder\n", file);
	else {
		if (isNewer) {
			char cmd[1024];
			sprintf(cmd, "cp %s %s\n", file, fileRef);
			puts(cmd);		
			system(cmd);
		}
		
	}
}

void expandFolder(const char* folder)
{

	FileNode* rootFn = FileNode::CreateRoot(folder);
	if(rootFn) {
		rootFn->expand();
		rootFn->print(1);
		delete rootFn;
	}
	

}
#define TEST_COMPARE
#ifdef TEST_EXPAND	

int main(int argc, char *argv[])
{
	const char* pFolder1 = (argc > 1)?argv[1]:".";

	expandFolder(pFolder1);
    return 0;

}
#endif

#ifdef TEST_COMPARE
int main(int argc, char *argv[])
{
    const char* pFolder1;
    const char* pFolder2;
	if (argc <2)
	{
		usage(argv[0]);
		exit(-1);
	}
	FileNode* rootFn1 = NULL;
	FileNode* rootFn2 = NULL;
	pFolder1 = argv[1];
	pFolder2 = (argc > 2)?argv[2]:".";
	
	do {
	    rootFn1 = FileNode::CreateRoot(pFolder1);
	    rootFn2 = FileNode::CreateRoot(pFolder2);
		if (!rootFn1 || !rootFn2)
			break;
		printf("expand folders...\n");
		rootFn1->expand();
		rootFn2->expand();
		printf("comparing...\n");
		rootFn1->compareAll(rootFn2, ComapreCallBack);

	} while(0);
	
    if(rootFn2) delete rootFn2; 
    if(rootFn1) delete rootFn1; 

    return 0;
}
#endif
