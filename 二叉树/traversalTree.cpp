#include <stdio.h>
#include <stdlib.h>

// 二叉树的遍历：前序，中序，后续，层序
// 二叉树的特性：
// 1. 第n层，最多有2^(n-1)个元素
// 2. N层树，总共最多有2^N - 1个元素
// 3. 完全二叉树中，节点i的父节点是i/2，左子节点是2i，右子节点是2i+1

// 遍历如下二叉树
/*
       a
     /   \
    b     c
  /   \
 d     f
  \   /
   e g
*/


struct treeNode {
    char v;
    struct treeNode* lChild;
    struct treeNode* rChild;
};

//先要做的是实现一个栈和入栈和出栈操作。
#define STACKSIZE 1024
static int curStackSize = STACKSIZE;
static treeNode** stack = NULL;
static int stackTop = -1;

void initStack()
{
    stack = (treeNode**)malloc(curStackSize * sizeof(treeNode*));
    if (!stack)
        exit(-1);
}

void push(treeNode* v)
{
    if (stackTop >= curStackSize - 1) {
        treeNode** temp = (treeNode**)realloc(stack, 2 * curStackSize * sizeof(treeNode*));
        if (!temp)
            exit(-1);
        curStackSize *= 2;
        stack = temp;
    }
    stack[++stackTop] = v;
}

treeNode* pop()
{
    if (stackTop >= 0)
        return stack[stackTop--];
    else
        return NULL;
}

/*
 *
 * 前序遍历：
 * 1. 根压入栈
 * 2. while 栈不为空：
 *      1. 取栈顶B，打印
 *      2. 如果有右节点，压入栈
 *      3. 如果有左节点，压入栈
 */
void preorder(treeNode *root)
{
    push(root);

    while (stackTop >= 0) {
        treeNode *p = pop();
        printf("%c", p->v);
        if (p->rChild)
            push(p->rChild);
        if (p->lChild)
            push(p->lChild);
    }
    printf("\n");
}

/*
 *
 * 中序遍历：
 * while root不为空 或者 堆栈不为空
 *      如果root不为空，压入栈，然后root指向左子节点
 *      如果root为空，取栈顶，赋值给root。打印root，然后root指向右子节点
 */
void inorder(treeNode *root)
{
    while(root || stackTop >= 0) {
        if (root) {
            push(root);
            root = root->lChild;
        } else {
            root = pop();
            printf("%c", root->v);
            root = root->rChild;
        }
    }
    printf("\n");
}

/*
 * 后续遍历
 * while root不为空 或者 堆栈不为空
 *      如果root不为空，压入栈，root指向左节点
 *      如果root为空，取栈顶赋值给root
 *          如果前一次打印的节点是当前root的右节点，或者右节点是空，说明root的右子树已经访问或没必要访问。打印root，更新preNode，置root空.
 *          否则root压入栈，然后root指向root的右子节点
 */
void postorder(treeNode *root)
{
    treeNode* preNode = NULL;
    while (root || stackTop >= 0) {
        if (root) {
            push(root);
            root = root->lChild;
        } else {
            root = pop();
            if (preNode == root->rChild || !root->rChild) {
                printf("%c", root->v);
                preNode = root;
                root = NULL;
            } else {
                push(root);
                root = root->rChild;
            }
        }
    }
    printf("\n");
}

int main()
{
    treeNode a,b,c,d,e,f,g;
    a.lChild = &b;
    a.rChild = &c;
    a.v = 'a';
    b.lChild = &d;
    b.rChild = &f;
    b.v = 'b';
    c.lChild = NULL;
    c.rChild = NULL;
    c.v = 'c';
    d.rChild = &e;
    d.lChild = NULL;
    d.v = 'd';
    e.lChild = NULL;
    e.rChild = NULL;
    e.v = 'e';
    f.lChild = &g;
    f.rChild = NULL;
    f.v = 'f';
    g.lChild = NULL;
    g.rChild = NULL;
    g.v = 'g';

    printf("前序遍历：abdefgc\n");
    printf("中序遍历：debgfac\n");
    printf("后序遍历：edgfbca\n");

    initStack();
    preorder(&a);
    inorder(&a);
    postorder(&a);
    free(stack);
    stack = NULL;
}
