/*
author:zhyq 
class:算法一班
time:2014-06-24
description:红黑树的实现
*/
#include <stdio.h>
#include <stdlib.h>

#define RED  0
#define BLACK  1
#define key_t int
#define data_t int
#define NIL -1
//节点定义
typedef struct node_t{
	struct node_t* left, *right, *parent;
	key_t key, colour;//关键字 颜色
}node_t;

node_t *nil;
//
node_t* Init(key_t key);
//生成关键字为key的节点，并返回
node_t* getNode(node_t* parent, key_t key);
//插入关键字为key的一个节点
node_t* Insert(node_t *root,key_t key);
//查找关键字为key的节点，并返回
node_t* Search(node_t *root,node_t ** parent,key_t key);
//删除关键子字为key的节点
node_t * Delete(node_t *root,key_t key);
//右平衡 具体详细原理，请看本实验实现的avl树（avlTree.c）
node_t* rightBlance(node_t *root,node_t* node);
//左平衡
node_t* leftBlance(node_t *root,node_t* node);
void printLevel(node_t *root);
void show_rb_tree(node_t * root);

node_t* getNode(node_t* parent, key_t key){
	node_t *temp_node = (node_t*)malloc(sizeof(node_t));
	temp_node->key = key;
	temp_node->colour = RED;
	temp_node->parent = parent;
	temp_node->left = temp_node->right = NULL;
	return temp_node;
}
node_t * Init(key_t key)
{
	//初始化叶子节点
	nil=(node_t *)malloc(sizeof(node_t));
	//初始化根节点
	node_t *root=(node_t *)malloc(sizeof(node_t));
	if(root == NULL || nil==NULL)
	{
		printf("初始化树时，分配内存失败！\n");
		system("pause");
		return NULL;
	}

	nil->parent=nil->left=nil->right=NULL;
	nil->colour=BLACK;
	nil->key=NIL;

	root->parent=root->left=root->right=NULL;
	root->key=key;
	root->colour=BLACK;
	return root;
}

void levelTraverse(node_t * root,int level)
{
	int i;
	for(i=0;i<level;i++)
		printf("   ");
	if(root == NULL)
	{
		printf("nil\n");
		return ;
	}
	else
	{
		printf("%d",root->key);
		if(root->colour == RED)
			printf("R\n");
		else printf("B\n");
	}
	level++;
	levelTraverse(root->left,level);
	levelTraverse(root->right,level);
}
void printLevel(node_t *root)
{
	int level;
	printf("\n层次遍历为：\n");
	for(level=1;level<=10;level++)
	{
		printf("L%d ",level);
	}
	printf("\n");
	levelTraverse(root,0);
	printf("\n");
}
node_t *Search(node_t *root,node_t** parent,key_t key)
{
	node_t *temp_node = root;
	node_t * temp_parent=NULL;
	if(root == NULL)
	{
		*parent=NULL;
		return NULL;
	}
	*parent=temp_node->parent;
	while(temp_node != NULL)
	{
		if(key == temp_node->key)
		{
			return temp_node;
		}
		*parent=temp_node;
		if(key < temp_node->key)
		{
			temp_node = temp_node->left; 
		}

		else 
		{
			temp_node = temp_node->right; 
		}

	}
	return temp_node;
}
/*
          A                  B
         / \               /   \
        B   a             C     A
       / \        =>     / \   / \
	  C   b             c1 c2  b a
     / \xuanzhuan
    c1 c2
*/
node_t* leftBlance(node_t *root,node_t* node)
{
	if(node == NULL || node->right == NULL)
		return NULL;

	node_t *parent_node=node, *child_node=node->right, *child_child_node=node->right->left;
	if(parent_node->parent != NULL)
	{
		if(parent_node->parent->left == parent_node)
			parent_node->parent->left = child_node;
		else 
			parent_node->parent->right = child_node;
	}
	else if(parent_node == root)
	{
		root = child_node;
	}
	child_node->parent = parent_node->parent;

	parent_node->parent = child_node;
	child_node->left = parent_node;

	parent_node->right = child_child_node;
	if(child_child_node != NULL)
		child_child_node->parent = parent_node;
	//debug
	//printLevel(child_node);
	return child_node;
}
node_t* rightBlance(node_t *root,node_t* node)
{
	if(node == NULL || node->left == NULL)
		return NULL;

	node_t *parent_node=node, *child_node=node->left, *child_child_node=node->left->right;
	if(parent_node->parent != NULL)
	{
		if(parent_node->parent->left == parent_node)
			parent_node->parent->left = child_node;
		else
			parent_node->parent->right = child_node;
	}
	else if(parent_node == root)
	{
		root = child_node;
	}
	child_node->parent = parent_node->parent;

	parent_node->parent = child_node;
	child_node->right = parent_node;
	//
	parent_node->left = child_child_node;
	if(child_child_node != NULL)
		child_child_node->parent = parent_node;
	
	//debug
	//printLevel(child_node);
	return child_node; 
}
/*
	知识准备：
	红黑树的性质：
	1 一个节点不红既黑   3 叶子节点是黑色 5各路径上黑高一致 
	2 根节点是黑色  
	4 如果一个节点是红色，那么他的两个孩子都为黑色 
	插入时将插入点设置为红色( 假设p(z) == p[p[z]]->left 另一情况)：
		1 case 1: 如果插入的点是根，则违反 性质2 ，直接将其设置黑色即可
		2 如果 插入点（z）的父节点也为红色(其叔叔为y)
			2.1  case 2.1: 如果y为红色，将p[z],y调为黑色。将p[p[z]]调为红色。把z=p[p[z]]。继续检查
			2.2  case 2.2: 如果y为黑色，z为右孩子，将z进行左旋（z和 p[z] 调换），之后转为case 3
			2.3  case 2.3: 如果y为黑色，z为左孩子，进行一次右旋 bingo。

*/
node_t * Insert(node_t *root,key_t key)
{
	node_t *parent_node=NULL;
	node_t *insert_node=Search(root,&parent_node,key);
	node_t *uncle_node=NULL;
	//如果是空节点，则生成一个根节点，并返回
	//case 1
	if(root == NULL)
	{
		root = getNode(NULL, key);
		root->colour = BLACK;
		return root;
	}
	//将节点插入到 parent的孩子处
	//如果这样的点已经存在，怎返回 
	if(insert_node != NULL)
	{
		return root;
	}    

	//如果要插入的节点关键字小于parent 则插到左边 
	if(key < parent_node->key)
	{
		insert_node = getNode(parent_node, key);
		parent_node->left=insert_node;
	}

	else
	{
		insert_node = getNode(parent_node, key);
		parent_node->right=insert_node;
	}
	node_t * cur_node=insert_node;
	node_t * cur_parent=parent_node;
	node_t * cur_uncle=NULL;
	//至此插入完成，现在进行调整，使得继续满足红黑树
	while(cur_node->parent != NULL && cur_node->parent->colour == RED)
	{
		cur_parent=cur_node->parent;
		//如果插入点父亲是一个左孩子
		if(cur_node->parent == cur_node->parent->parent->left)
		{//p[z]=p[p[z]]->left
			//叔叔是红色 case 2.1
			if(cur_node->parent->parent->right!=NULL && cur_node->parent->parent->right->colour == RED)
			{
				//把父亲和叔叔置为黑色
				cur_node->parent->colour=BLACK;
				cur_node->parent->parent->right->colour=BLACK;
				//把父亲的父亲置为红色
				cur_node->parent->parent->colour=RED;
				//z 跳到它的爷爷处
				cur_node=cur_node->parent->parent;
				continue;
			}
			//case 2.2 叔叔是黑色，且自己为右孩子
			else if(cur_node == cur_node->parent->right)
			{
				cur_node=cur_node->parent;
				root=leftBlance(root,cur_node);
			}
			//debug 
			printLevel(root);
			//case 2.3 叔叔是黑色，自己为左孩子
			cur_node->parent->colour=BLACK;
			cur_node->parent->parent->colour=RED;
			root=rightBlance(root,cur_node->parent->parent);

			printLevel(root);
		}

		else//插入点的父亲是一个右孩子，和上面分析方法一样
		{//p[z]=right[p[p[z]]];
			//叔叔是红色 case 2.1
			//叔叔是红色 case 2.1
			if( cur_node->parent->parent->left!= NULL && cur_node->parent->parent->left->colour == RED )
			{
				//把父亲和叔叔置为黑色
				cur_node->parent->colour=BLACK;
				cur_node->parent->parent->left->colour=BLACK;
				//把父亲的父亲置为红色
				cur_node->parent->parent->colour=RED;
				//z 跳到它的爷爷处
				cur_node=cur_node->parent->parent;
				continue;
			}
			//case 2.2 叔叔是黑色，且自己为右孩子
			else if(cur_node == cur_node->parent->left)
			{
				cur_node=cur_node->parent;
				root=rightBlance(root,cur_node);
			}
			//case 2.3 叔叔是黑色，自己为左孩子
			cur_node->parent->colour=BLACK;
			cur_node->parent->parent->colour=RED;
			root=leftBlance(root,cur_node->parent->parent);       
		}
	}


	//满足 parent 不为空（还没走到根节点） 并且还没找到黑父节点，需要继续往上调整 
	//z为 z插入点的父亲，如果插入点的父亲为红色 
	root->colour = BLACK;
	return root;
}
/*
  删除规则：
   case 1 : x的兄弟w是红色的
   case 2 : x的兄弟w是黑色的，而且w的两个孩子都是黑色的
   case 3 : x的兄弟w是黑色的，w的左孩子为红色，右孩子为黑色
   case 4 : x的兄弟w是黑色的，而且w的右孩子为红色
*/

node_t * Delete(node_t * root,int key)
{
	node_t * parent=NULL;
	node_t * temp_node = Search(root,&parent,key), *del_node, *parent_node, *child_node, *brother_node;
	int temp_key;
    //要删除的节点不存在，则直接额返回根
	if(temp_node == NULL) return root;

	del_node = temp_node;
    //如果temp_node的孩子左右孩子都不为空，则把temp_node的后继补上
	if(temp_node->left != NULL && temp_node->right != NULL)
	{//找它的后继，就是右子树的最左下角哪个点 right->left->left->left
		del_node = temp_node->right;
		while(del_node->left != NULL)
		{
			del_node = del_node->left;
		}
		temp_key = del_node->key;
		del_node->key = temp_node->key;
		temp_node->key = temp_key;
	}

	//到这里，del_node是要删除的节点 
	if(del_node->left != NULL)
		child_node = del_node->left;
	else 
		child_node = del_node->right;
    
	parent_node = del_node->parent;
	if(parent_node != NULL)
	{
		//把del_node的链接关系从红黑树中删除
		if(parent_node->left == del_node)
			parent_node->left = child_node;
		else
			parent_node->right = child_node;
	}
	else
	{
		//如果要删除的节点是根节点，把它的孩子节点赋值为根节点
		root = child_node;
		free((void*)del_node);
		return root;
	}

	//如果要删除的节点不是根节点，且是红色的，则只需删除即可，并没有破坏红黑树的任何性质.
	if(del_node->colour == RED)
	{
		free((void*)del_node);
		return root;
	}

	free((void*)del_node);
	del_node = child_node;

	//现在开始调整
	while(del_node != root)
	{
		if(del_node != NULL && del_node->colour == RED)
		{
			//如果删除的节点为红色，无需调整
			del_node->colour = BLACK;
			return root;
		}
        //获取del_node的兄弟节点brother_node(w节点)
		if(del_node == parent_node->left)
			brother_node = parent_node->right;
		else 
			brother_node = parent_node->left;

		//printf("%d\n", brother_node->key);

		
		if(brother_node->colour == BLACK)
		{
			//case 3 兄弟是黑色的，且兄弟的左孩子红色
			if(brother_node->left != NULL && brother_node->left->colour == RED)
			{
				if(del_node == parent_node->left)
				{
                    //右旋转 得到 case 4 w是黑色，w右孩子为红色
					brother_node = rightBlance(root,brother_node);
					brother_node->colour = BLACK;
					brother_node->right->colour = RED;

					parent_node = leftBlance(root,parent_node);
					parent_node->colour = parent_node->left->colour;
					parent_node->left->colour = BLACK;
					parent_node->right->colour = BLACK;
				}
				else
				{
					parent_node = rightBlance(root,parent_node);
					parent_node->colour = parent_node->right->colour;
					parent_node->right->colour = BLACK;
					parent_node->left->colour = BLACK;
				}

				return root;
			}
            //case 4  w是黑色，w右孩子为红色
			else if(brother_node->right != NULL && brother_node->right->colour == RED)
			{
				if(del_node == parent_node->right)
				{

					brother_node = leftBlance(root,brother_node);
					brother_node->colour = BLACK;
					brother_node->left->colour = RED;

					parent_node = rightBlance(root,parent_node);
					parent_node->colour = parent_node->right->colour;
					parent_node->right->colour = BLACK;
					parent_node->left->colour = BLACK;
				}
				else
				{
					parent_node = leftBlance(root,parent_node);
					parent_node->colour = parent_node->left->colour;
					parent_node->left->colour = BLACK;
					parent_node->right->colour = BLACK;
				}        
				return root;
			}
			else{//case 1 : w是红色的
				brother_node->colour = RED;
				del_node = parent_node;
				parent_node = del_node->parent;
				continue;
			}
		}
		else
		{
			if(del_node == parent_node->left)
			{
				parent_node = leftBlance(root,parent_node);
				parent_node->colour = BLACK;
				parent_node->left->colour = RED;
				parent_node = parent_node->left;
			}
			else
			{
				parent_node = rightBlance(root,parent_node);
				parent_node->colour = BLACK;
				parent_node->right->colour = RED;
				parent_node = parent_node->right;
			}
		}
	}
	root->colour = BLACK;
	return root;
}

int main()
{
	int key;
	int option;
	node_t *root=NULL;
	printf("请输入根节点的关键字:  ");
	scanf("%d",&key);
	root=Init(key);
	printLevel(root);
	while(1)
	{
		printf("输入1表示插入，2表示删除，其他键表示退出：  ");
		scanf("%d",&option);
		if(option==1)
		{
			printf("请输入要插入节点的关键字： ");
			scanf("%d",&key);
			root=Insert(root,key);
			printLevel(root);
			printf("\n");
		}
		else if(option == 2)
		{
			printf("请输入要删除节点的关键字：  ");
			scanf("%d",&key);
			Delete(root,key);
			printLevel(root);
			printf("\n");
		}
		else 
			break;
	}
	return 0;
}


