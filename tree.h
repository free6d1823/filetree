#ifndef TREE_H_
#define TREE_H_

class Tree{
public:
    Tree();
    ~Tree();
    int getDepth();
    /* tree operation */
    Tree* getRoor();
    /* append a node after current child */
    Tree* appendChild(Tree* node);

    /* insert this node after a node*/
    Tree* insert(Tree* node);
    /* remove this node from current tree */
    Tree* remove();



     /* number of children */
    int children();
protected:
    void setDepth(int depth);
    Tree* mParent;
    Tree* mPrev;
    Tree* mNext;
    Tree* mFirst;
    Tree* mLast;
    int mDepth;
};
#endif //TREE_H
