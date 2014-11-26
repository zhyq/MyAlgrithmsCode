/*
  author:zhyq
  class:算法一班
  time:2014-06-20
  description:实现平衡查找树
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<queue>
#define LEFTHIGH -1
#define RIGHTHIGH 1
#define EQUALHIGH 0

#define INSERT 1
#define DELETE 2

typedef int key_t;// 关键字类型
typedef int data_t;//数据类型
typedef struct node_t {//节点类型
    struct node_t * left,*right,*parent;
    key_t key;
    data_t data;
    //左子树减，右子树加，如果为-2表示左子树比右子树高2。 同样如果是2，表示右子树比左子树高2.
    int balance;
}node_t;

//先序遍历
void preorderTraverse(node_t * root)
{
    if(root != NULL)
    {
        printf("%d->",root->key);
        preorderTraverse(root->left);
        preorderTraverse(root->right);
    }
}
//中序遍历，是从小到大顺序
void inorderTraverse(node_t * root)
{
    if(root != NULL)
    {
        inorderTraverse(root->left);
        printf("%d->",root->key);
        inorderTraverse(root->right);
    }
}
//后序遍历
void lastorderTraverse(node_t * root)
{
    if(root != NULL)
    {
        lastorderTraverse(root->left);
        lastorderTraverse(root->right);
        printf("%d->",root->key);
    }
}
//层次遍历
void levelTraverse(node_t * root,int level)
{
    int i;
    for(i=0;i<level;i++)
       printf("   ");
    if(root == NULL)
    {
        printf("nil,\n");
        return ;
    }
    else
    {
        printf("%d\n",root->key);
    }
    level++;
    levelTraverse(root->left,level);
    levelTraverse(root->right,level);
}
//左平衡调整 LL LR 左子树高 此是root是树根，parent是需要调整的子树的根节点，child为其左孩子
node_t * leftBlance(node_t * root,node_t * parent,node_t * child)
{
    node_t *cur;
    assert((parent != NULL)&&(child != NULL));
    //LR型旋转 如果右孩子高，说明 父节点左孩子高，左孩子中又是右孩子高，所以是LR算法。
    if (child->balance == RIGHTHIGH)
    {//需要调整的三个点为 parent child cur 关系为:parent->left==child  child->right==cur
        // 分别对应如下：parent:A  child:B  cur:C 
        /*
                   A                                C
                  / \                             /   \ 
                 B   a                           B     A
                / \           =>                / \   / \
               b   C                           b  c1  c2 a
                  / \
                 c1 c2
        */

        cur = child->right;
        cur->parent = parent->parent;
        child->right = cur->left;
        if (cur->left != NULL)
        {
            cur->left->parent = child;
        }
        parent->left = cur->right;
        if (cur->right != NULL)
        {
            cur->right->parent = parent;
        }
        cur->left = child;
        cur->right = parent;
        child->parent = cur;
 
        if (parent->parent != NULL)
        {
            if (parent->parent->left == parent)
            {
                parent->parent->left = cur;
            }
            else
            {
                parent->parent->right = cur;
            }
        }
        else
        {
            root = cur;
        }
        parent->parent = cur;
        if (cur->balance == EQUALHIGH)
        {
            parent->balance = EQUALHIGH;
            child->balance = EQUALHIGH;
        }
        else if (cur->balance == RIGHTHIGH)
        {
            parent->balance = EQUALHIGH;
            child->balance = LEFTHIGH;
        }
        else
        {
            parent->balance = RIGHTHIGH;
            child->balance = EQUALHIGH;
        }
        cur->balance = EQUALHIGH;
    }
    ///LL型旋转 父节点左孩子高，左孩子又是左孩子高，就是LL型旋转
            /* 

                  A                              B
                 / \                           /   \ 
                B   a                         C     A
               / \         =>                / \    /\
              C   b                         c1 c2  b  a
             / \
            c1 c2
           
         */
    else
    {
        child->parent = parent->parent;
        parent->left = child->right;
        if (child->right != NULL)
        {
            child->right->parent = parent;
        }
        child->right = parent;
        if (parent->parent != NULL)
        {
            if (parent->parent->left == parent)
            {
                parent->parent->left = child;
            }
            else
            {
                parent->parent->right = child;
            }
        }
        else
        {
            root = child;
        }
        parent->parent = child;
        // 插入时调用
        if (child->balance == LEFTHIGH) 
        {
            child->balance = EQUALHIGH;
            parent->balance = EQUALHIGH;
        }
        //删除时 调用
        else
        {
            child->balance = RIGHTHIGH;
            parent->balance = LEFTHIGH;
        }
    }
    return root;
}
//右平衡调整 RL RR
node_t * rightBlance(node_t * root,node_t * parent,node_t * child)
{
    node_t *cur;
    assert((parent != NULL)&&(child != NULL));
    // RL型
    
    if (child->balance == LEFTHIGH) 
    {
        cur = child->left;
        cur->parent = parent->parent;
        child->left = cur->right;
        if (cur->right != NULL)
        {
            cur->right->parent = child;
        }
        parent->right = cur->left;
        if (cur->left != NULL)
        {
            cur->left->parent = parent;
        }
        cur->left = parent;
        cur->right = child;
        child->parent = cur;
        if (parent->parent != NULL)
        {
            if (parent->parent->left == parent)
            {
                parent->parent->left = cur;
            }
            else
            {
                parent->parent->right = cur;
            }
        }
        else
        {
            root = cur;
        }
        parent->parent = cur;
        if (cur->balance == EQUALHIGH)
        {
            parent->balance = EQUALHIGH;
            child->balance = EQUALHIGH;
        }
        else if (cur->balance == LEFTHIGH)
        {
            parent->balance = EQUALHIGH;
            child->balance = RIGHTHIGH;
        }
        else
        {
            parent->balance = LEFTHIGH;
            child->balance = EQUALHIGH;
        }
        cur->balance = EQUALHIGH;
    }
    // RR型 
    else
    {
        child->parent = parent->parent;
        parent->right = child->left;
        if (child->left != NULL)
            child->left->parent = parent;
        child->left = parent;
        if (parent->parent != NULL)
        {
            if (parent->parent->left == parent)
            {
                parent->parent->left = child;
            }
            else
            {
                parent->parent->right = child;
            }
        }
        else
        {
            root = child;
        }
        parent->parent = child;
        //插入时调用
        if (child->balance == RIGHTHIGH) 
        {
            child->balance = EQUALHIGH;
            parent->balance = EQUALHIGH;
        }
        //删除时调用
        else
        {
            child->balance = LEFTHIGH;
            parent->balance = RIGHTHIGH;
        }
    }
    return root;

}
//查找函数 返回查找到的节点 parent 保存找到节点的父节点,或者没有找到,保存访问的最后这个叶子节点,如果找到的是root节点,返回空
node_t * Search(node_t * root,key_t key,node_t ** parent);
//插入函数 返回跟节点
node_t * Insert(node_t** root,node_t * ins_node);
//删除函数 返回要刷除的节点
node_t * Delete(node_t ** root,key_t key);
//获取最小的节点 最左下角的哪个节点
node_t * minNode(node_t *root)
{
    if(root == NULL)
        return NULL;
    node_t * temp_node=root;
    while(temp_node->left != NULL)
        temp_node=temp_node->left;
    return temp_node;
}
//获取最大节点，最右下角那个点
node_t * maxNode(node_t *root)
{
    if(root == NULL)
        return NULL;
    node_t * temp_node=root;
    while(temp_node->right != NULL)
    {
        temp_node=temp_node->right;
    }
    return temp_node;
}
//找出当前node的中序遍历的前一个node
//1 找此节点的左孩子为根的子树最右下的那个节点 node->left->right->right->right ...//2 如果是没有左孩子，找它所在右子树的父亲 
node_t * preNode(node_t * node)
{
    node_t *curr=NULL;

    if(node==NULL)
        return NULL;

    if(node->left != NULL)
    {
        curr=node->left;
        //找此节点的左子树最右下的那个节点
        while(curr->right != NULL)
            curr=curr->right;
        return curr;
    }
    curr=node;
    //如果node有父节点，且node不是其父节点的右孩子，就一直找它的父节点
    while(curr->parent != NULL && ( curr != curr->parent->right ) )
    {
        curr=curr->parent;
    }
    return curr->parent;
}
//找出当前node的中序遍历的后一个node
//1 此节点的后继为其右子树最左下的哪个节点 node->right->left->left->left  //2 找它所在左子树的根
node_t * nextNode(node_t *node)
{
    node_t * curr=NULL;
    
    if(node == NULL)
        return NULL;
    if(node->right != NULL)
    {
        node_t * curr=node->right;
        while(curr->left != NULL)
            curr=curr->left;
        return curr;
    }
    curr=node;
    while(curr->parent != NULL && ( curr!=curr->parent->left) )
    {
        curr=curr->parent;
    }
    return curr->parent;
}
node_t * Search(node_t * root,key_t key,node_t ** parent)
{
    node_t * temp_node=root;
    //如果树空
    if(root==NULL)
    {
        *parent=NULL;
        return NULL;
    }
    
    *parent=root->parent;
    while(temp_node != NULL)
    {
        
        //关键字相等，查找成功
        if(temp_node->key == key)
        {        
            return temp_node;
        }
        *parent=temp_node;
        //要查找的关键字大于此节点的关键字，查找左子树
        if(temp_node->key > key)
            temp_node=temp_node->left;
        //要查找的关键字小于此节点的关键字，查找右子树
        else if(temp_node->key < key)
            temp_node=temp_node->right;
    }
}
//插入时，如果出现失衡，需要进行调整
node_t * Insert(node_t *root,key_t ins_key)
{
    // printf("start insert\n");
    node_t * temp_node=(node_t*)malloc(sizeof(node_t));
    if(temp_node == NULL)
    {
        printf("为新节点分配内存失败！\n");
        system("pause");
        //exit -1;
    }
    node_t * parent_node=NULL;//需要插入节点的父节点
    node_t * child_node=NULL;
    

    //如果是空树 插入到跟节点处
    if(root == NULL)
    {
        temp_node->parent = NULL;
        return temp_node;
    }

    //如果当前树中含有此节点,返回根
    if(Search(root,ins_key,&parent_node) != NULL)
    {
        return root;
    }
    temp_node->key=ins_key;//要插入的节点
    temp_node->balance=EQUALHIGH;
    temp_node->left = temp_node->right=NULL;
    temp_node->parent = parent_node;
    //printf("ins parent %d\n",parent_node->key);
    //如果需要插入的节点大于 其父节点,则它为父节点的右孩子
    if(temp_node->key > parent_node->key)
    {
        parent_node->right=temp_node;
        child_node=parent_node->right;
    }
    else
    {
        parent_node->left=temp_node;
        child_node=parent_node->left;
    }

    while(parent_node != NULL)
    {
        // printf("start balance\n");
        //如果插入的节点是左节点
        if(parent_node->left == child_node)
        {
            //如果右子树高，插入左子树后，不会引起失衡（祖父节点也不会失衡，因为子树高度不变）
            if(parent_node->balance == RIGHTHIGH)
            {
                //标记高度相等
                // printf("insert RH\n");
                parent_node->balance = EQUALHIGH;
                return root;
            }
             //左子树比右子树高，插入左子树后，此点失去平衡
            else if(parent_node->balance ==  LEFTHIGH)
            {
                //todo 左子树高，需要进行平衡操作 LL
                //printf("insert LH\n");
                 root=leftBlance(root,parent_node,child_node);
                break; 
            }
           //如果左右子树高度相等，插入左子树后，其祖父节点可能失去平衡（因为子树增高）。
            else if(parent_node->balance == EQUALHIGH)
            {   
                //todo 检测和调整祖父节点平衡
                //printf("insert EQ\n");
                parent_node->balance=LEFTHIGH;
                child_node=parent_node;
                parent_node=parent_node->parent;
            }
        }

        else if(parent_node->right == child_node)
        {
            // printf("insert right \n");
            //如果左子树高，插入右子树后，不会引起失衡（祖父节点也不会失衡，因为子树高度不变）
            if(parent_node->balance == LEFTHIGH)
            {
                //标记高度相等
                //   printf("right LH\n");
                parent_node->balance=EQUALHIGH;
                return root;
            }
                
            //右子树比左子树至高,插入右子树后，此节点失去平衡（祖父节点不会失衡，因为子树高度不变）
            else if(parent_node->balance == RIGHTHIGH)
            {
                //todo 右子树高，需要进行左平衡操作
                //RR
                //  printf("right RH\n");
                root=rightBlance(root,parent_node,child_node);
                break;
            }
                
            //如果左右子树高度相等，插入右子树后，其祖父节点可能失去平衡（因为子树增高）。
            else if(parent_node->balance == EQUALHIGH)
            {
                //  printf("right EQ\n");
                parent_node->balance = RIGHTHIGH;
                child_node=parent_node;
                parent_node=parent_node->parent;
                //todo 检测和调整祖父节点平衡
            }        
        }
    }
    return root;
}
//删除时，分叶子节点和非叶子节点，失去平衡时需要调整。
node_t * Delete(node_t * root,key_t key)
{
    node_t *del_node=NULL;//需要删除的节点
    node_t * del_node_parent=NULL;//要删除的节点所在的父节点
    node_t * del_node_child=NULL;
    key_t temp_key;
    //如果是空树，直接返回
    if(root == NULL)
    {
        return NULL;
    }
    //调用查找，找到需要删除的节点
    del_node=Search(root,key,&del_node_parent);

    //树中不存在这样一个要删除的节点，之间返回null
    if(del_node == NULL)
    {
        return root;
    }

    //如果要删除的点存在，且其父节点为NULL，说明要删除的点为根节点
    if(del_node_parent==NULL)
    {
        del_node=root;
    }

    
    del_node_child=del_node;

    //此处由于没有找到删除节点后找到其相对应的后继点，卡了许久：
    /*
      删除规则：
      后继补上规则(B:补)：
      B1 如果删除节点为叶子节点，直接删除
      B2 如果删除节点有一个孩子域为空，把它的孩子补上
      B3 如果要删除的节点两个孩子都不空
         B3.1 如果该节点平衡或者 左子树高，则找到左子树中具有最大值的max节点补上（补前驱）
         B3.2 如果右子树高，则找到右节点的最小值min补上。（补后继）

      调整规则（T：调）：
      T1 对于B1 B2 都会引起树删除点所在的parent节点为根节点的子树高度降1，需要回溯往前处理
      T2 对于B3，如果要删除节点的父节点平衡因子为0，高度不会变化，不需调整。
         
     */
    //case B1 叶子节点
    if(del_node->left == NULL && del_node->right == NULL)
    {
        
    }
    //case B2 B3 其实处理方法相同，B2为B3的特殊情况，所以，在这里统一处理
     if ((del_node->left != NULL)||(del_node->right != NULL)) //确定需要删除的结点
    {
        //如果左子树高，找左子树最大值，也就是需要删除节点的前驱
        if (del_node->balance == LEFTHIGH)
        {
            del_node_child = preNode(del_node);
        }
        //否则右子树高，找右子树的最小值，也就是要删除节点的后继
        else
        {
            del_node_child = nextNode(del_node);
        }
        //找到后把后继点的相关信息填到这点上面，相当与补充上 （这里交换关键字值）
        temp_key = del_node_child->key;
        del_node_child->key = del_node->key;
        del_node->key = temp_key;
        del_node = del_node_child;
    }
    
    
    del_node_child=del_node;
    del_node_parent=del_node->parent;
  
    //找出需要调整的最小子树
    while(del_node_parent != NULL)
    {
        //如果要删除是左孩子
        if(del_node_child == del_node_parent->left)
        {
            //左分支高,那么删除左孩子后，就一样高了，但是子树高度-1 ，可能会导致祖父节点失衡
            if(del_node_parent->balance == LEFTHIGH)
            {
                del_node_parent->balance=EQUALHIGH;
                del_node_child=del_node_parent;
                del_node_parent=del_node_parent->parent;
            }
            //如果右子树高 删除左子树后，需要右调
            else if(del_node_parent->balance == RIGHTHIGH)
            {
                del_node_child=del_node_parent->right;
                //todo 右平衡
                root=rightBlance(root,del_node_parent,del_node_child);
                break;
            }
            //一样高,删除左节点，不会导致子树高度变化，所以不许要调整
            else
            {
                del_node_parent->balance=RIGHTHIGH;
                break;
            }
        }
        //要删除右子树
        else
        {
            //右分支高,那么删除右孩子后，就一样高了，但是子树高度-1 ，可能会导致祖父节点失衡
            if(del_node_parent->balance == RIGHTHIGH)
            {
                del_node_parent->balance=EQUALHIGH;
                del_node_child=del_node_parent;
                del_node_parent=del_node_parent->parent;
            }
            //如果左子树高
            else if(del_node_parent->balance == LEFTHIGH)
            {
                del_node_child=del_node_parent->left;
                //todo 右平衡
                root=leftBlance(root,del_node_parent,del_node_child);
                break;
            }
            //一样高,删除右节点，不会导致子树高度变化，所以不许要调整
            else
            {
                del_node_parent->balance=LEFTHIGH;
                break;
            }
        }
    }

    if(del_node->parent != NULL)
    {
        //要删除的节点在左孩子
        if(del_node == del_node->parent->left)
            del_node->parent->left=NULL;
        else del_node->parent->right=NULL;
    }
    free(del_node);
    del_node=NULL;
    if(root == del_node)
        root=NULL;
    return root;

}


node_t * createNode(key_t key)
{
    node_t * node=(node_t *)malloc(sizeof(node_t));
    node->left=node->right=node->parent=NULL;
    node->balance=EQUALHIGH;
    node->key=key;
}

void print(node_t  *root)
{
    printf("\n先序遍历为：");
    preorderTraverse(root);

    printf("\n中序遍历为：");
    inorderTraverse(root);

    printf("\n后序遍历为：");
    lastorderTraverse(root);
}

void printLevel(node_t *root)
{
    int level;
    printf("\n层次遍历为:\n");
    for(level=1;level<=10;level++)
    {
        printf("L%d ",level);
    }
    printf("\n");
    levelTraverse(root,0);
    printf("\n");
}

int main(void)
{
    int i;
    int option;
    //int data[]={1,2,3,4,5};
     int data[]={5,1,3,2,8,4,9,7,6};
    printf("插入顺序为：");
    for(i=0;i<sizeof(data)/sizeof(data[0]);i++)
        printf("%d ",data[i]);
    
    node_t * root=NULL;
    node_t temp_node;
    root=createNode(data[0]);
    for(i=1;i<sizeof(data)/sizeof(data[0]);i++)
    {
        root=Insert(root,data[i]);        
    }
    printf("\n");
    printLevel(root);
    print(root);
    printf("\n");
    //Delete(root,3);
    
    //  Delete(root,5);
    while(root != NULL )
    {
        key_t op_key;
        printf("请输入指令，1为插入，2为删除，其他输入为退出\n");
        scanf("%d",&option);
        if(option==1)
        {
            printf("请输入要插入节点的关键字：\t");
            scanf("%d",&op_key);
            Insert(root,op_key);
            printLevel(root);
            printf("\n");
        }
        else if(option == 2){
        printf("请输入要删除节点的关键字:\t");
        scanf("%d",&op_key);
        Delete(root,op_key);
        printLevel(root);
        printf("\n");
        }
        else break;
    }
}



//层次遍历2
/*
void levelTraverse2(node_t *root)
{
    int total_node=0;//记录所有点的个数，包括空节点
    int level=0;//记录二叉树的层次
    int cur_level=0;//打印是所在的层次
    std::queue<node_t>queue_node_traverse;
    std::queue<node_t>queue_node;
    node_t left,right,nil;
    nil.key=-1;
    node_t cur=*root;
    queue_node_traverse.push(cur);
    
    while(!queue_node_traverse.empty())
    {
        cur=queue_node_traverse.front();
        queue_node_traverse.pop();
        if(cur.left != NULL)
        {
            left=*cur.left;
            queue_node_traverse.push(left);
            queue_node.push(left);
        }
        else
            queue_node.push(nil);
        if(cur.right != NULL)
        {
            right=*cur.right;
            queue_node_traverse.push(right);
            queue_node.push(right);
        }
        else
            queue_node.push(nil);   
    }
    total_node=queue_node.size();
    while(total_node > 0)
    {
        total_node>>1;
        level++;
    }
    total_node=queue_node.size();
    //todo 开始层次打印
    char **content;
    int i,j;
    int array_size=2*total_node;
    content=(char **)malloc(sizeof(char *)*(array_size+1));
    for(i=0;i<=array_size;i++)
    {
        *content=(char *)malloc(sizeof(char)*(array_size+1));
    }
    
    int dist=level-cur_level;
    while(dist > 0)
    {
        for(i=dist;i>0;i--)
        {
            //for(j=0;j<)
        }
    }
 
    
}
*/

