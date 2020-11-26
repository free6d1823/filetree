/*## Tree class
*/
#include <string.h>
#include "tree.h"

Link::Link(void* data): mPrev(0), mNext(0)
{
	mData = data;
}
Link::~Link(){
	if (mNext)
		delete mNext;
	mNext = NULL;
}
Link* Link::append(void* data)
{
	Link* ln = new Link(data);
	mNext = ln;
	ln->mPrev = this;
	return ln;
}


Tree::Tree():
        mParent(0),mPrev(0), mNext(0), mFirst(0), mLast(0), mDepth(0)
{}
Tree::~Tree(){}
Tree* Tree::getRoor()
{
    Tree* pTop = this;
    while(pTop->mParent)
        pTop = pTop->mParent;
    return pTop;
}
Tree* Tree::appendChild(Tree* node)
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
int Tree::children()
{
    int c = 0;
    Tree* nd = mFirst;
    while (nd){
        c++;
        nd = nd->mNext;
    }
    return c;
}
int Tree::getDepth(){ return mDepth;}

void Tree::setDepth(int depth)
{
    mDepth = depth;
}

/* insert this node after a node*/
Tree* Tree::insert(Tree* node)
{
    //siblings must be empty
    if (!node || mPrev != NULL || mNext != NULL|| mParent != NULL)
        return NULL;
    Tree* pLeft = node->mNext;
    mParent = node->mParent;
    mPrev = node;
    mNext = pLeft;
    node->mNext = this;
    if(pLeft) pLeft->mPrev = this;
    else mParent->mLast = this;
    return node;
     
}
/* remove this node fron current tree, this node becomes a root */
Tree* Tree::remove()
{
    if (mParent) {
        if (mParent->mFirst == this) mParent->mFirst = mNext;
        if(mParent->mLast == this) mParent->mLast = mPrev;
        mParent = NULL;
    }

    if (mPrev) {
        mPrev->mNext = mNext;
        if (mNext){ mNext->mPrev = mPrev; mNext = NULL;}
        mPrev = NULL;
    } else {
        if (mNext) {
            mNext->mPrev = NULL;
            mNext = NULL;
        }
    }
    return this;   
}
