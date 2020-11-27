#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "filetree.h"

int gOptions = 0;
#define OP_SHOW 0
#define OP_NEWER 1
#define OP_OOD 2

void usage(char* name)
{
    printf("Usage: %s [-d] <target_path> [source]\n", name);
    printf("Replace files in <source> with those in <target_path> that is newer.\n");
	printf("   -l       list target_path tree\n");
	printf("            only show difference\n");
	printf("   -n       copy if target_path is newer\n");
	printf("   -o       copy if target_path is newer and size is different\n");

}

void ComapreCallBack(FileNode* candidate, FileNode* refference)
{
	if(refference == NULL) {
		char* p1 = candidate->createFilePathName();
		printf("%s is only in the folder\n", p1);
		free(p1);
	}else {
		char* p1 = candidate->createFilePathName();
		char* p2 = refference->createFilePathName();
		bool bCopy = false;
		if (gOptions == OP_SHOW) {
			printf(">>-- %s\n", p1);
			candidate->print(2);
			printf("<<-- %s\n", p2);
			refference->print(2);

		} else if (gOptions == OP_NEWER) {
			bCopy = (candidate->mTime > refference->mTime);
		} else if (gOptions == OP_OOD) {
			bCopy = (candidate->mTime > refference->mTime)&&(candidate->mSize != refference->mTime);
		}
		if (bCopy) {

			char cmd[1024];
			sprintf(cmd, "cp -a %s %s\n", p1, p2);
			puts(cmd);
			int r =system(cmd);
			if (r!=0)
				fprintf(stderr, "cp error!\n");
		}
		free(p1);
		free(p2);
	}
}

void listFolder(const char* folder)
{

	FileNode* rootFn = FileNode::CreateRoot(folder);
	if(rootFn) {
		rootFn->expand();
		rootFn->print(1);
		delete rootFn;
	}
	

}

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
	if(argv[1][0] == '-') {
		if (argv[1][1] == 'l') {
			pFolder1 = (argc > 2)?argv[2]:".";
			listFolder(pFolder1);
			exit(0);
		}
		if (argv[1][1] == 'n') {
			gOptions = OP_NEWER;
		}else if (argv[1][1] == 'o') {//earlier
			gOptions = OP_OOD;
		}else { //
			gOptions = OP_SHOW; //show only
		}
		
        pFolder1 = argv[2];
        pFolder2 = (argc > 3)?argv[3]:".";

	} else {
		pFolder1 = argv[1];
		pFolder2 = (argc > 2)?argv[2]:".";
	}
	do {
	    rootFn1 = FileNode::CreateRoot(pFolder1);
	    rootFn2 = FileNode::CreateRoot(pFolder2);
		if (!rootFn1 || !rootFn2)
			break;
		printf("expand folder %s...\n", pFolder1);
		rootFn1->expand();
		rootFn2->expand();
		printf("comparing with %s...\n", pFolder2);
		rootFn1->compareAll(rootFn2, ComapreCallBack);

	} while(0);
	
    if(rootFn2) delete rootFn2; 
    if(rootFn1) delete rootFn1; 

    return 0;
}

