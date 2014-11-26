/*
time:2014-5-10
author:zhyq
description: dynamic about 0-1 package problem 
*/


#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#ifndef MY_DEFINE
#define MY_DEFINE
#define bool int
#define false 0
#define true 1
#define WSIZE 10//重量的范围
#define PSIZE 10//物品价值的范围
#endif

int n;//物品的件数
int *p=NULL;//指向物品价值
int *w=NULL;//指向物品重量
bool *sel=NULL;//标记物品是否被选中
int **c=NULL;//c[i][j]表示 从前i件物品中放重量为j 时的价值量
int m;//包能放下的最大重量
int rm;//real m 最后包装下的总重量
void init(void);//init
void pack(void);
void printSelectPack(void);
void init(void)
{
	int i;
	srand(time(NULL));
	printf("请输入包的最大载重:\n");
	scanf("%d",&m);
	printf("请输入物品的件数:\n");
	scanf("%d",&n);
	sel=(bool *)malloc(sizeof(bool)*(n+1));
	p=(int *)malloc(sizeof(int)* (n+1));
	w=(int *)malloc(sizeof(int)* (n+1));
	c=(int **)malloc(sizeof(int *)* (n+1));
	if(p==NULL || w==NULL || c==NULL || select==NULL)
	{	
		printf("分配内存失败\n");
		system("pause");
		exit(0);
	}
	for(i=0;i<=n;i++)
	{	
		c[i]=(int *)malloc(sizeof(int)*(m+1));
		if(c[i]==NULL)
		{
               		printf("分配内存失败\n");
		        system("pause");
	                exit(0);
		}
	}
	for(i=0;i<=n;i++)
		sel[i]=false;
	//printf("Please input the n object preasuer:\n");
	for(i=1;i<=n;i++)
		p[i]=random()%PSIZE+1;//scanf("%d",&p[i]);
	//printf("Please inputthe n object weight:\n");
	for(i=1;i<=n;i++)
		w[i]=random()%WSIZE+1;//scanf("%d",&w[i]);
	return ;
}
void pack(void)
{//c[i][m]=max{c[i-1][m],c[i-1][m-w[i]]+p[i]}//第i个取与不取，找最大值
	int i,j;
	for(i=0;i<=n;i++)
		c[i][0]=0;//当只能放下0个重量的物品时，总价值为0
	for(i=0;i<=m;i++)
		c[0][i]=0;//当没有物品可选时，总价值为0
	
	for(i=1;i<=n;i++)
	{//w[i][j]表示从前i件产品选择总质量为j的价值
		for(j=1;j<=m;j++)
		{
			if(w[i]<j)//表示第i件物品可以放下
			{	
				if( c[i-1][j] > c[i-1][ j-w[i] ]+p[i] )
					c[i][j]=c[i-1][j];//不放第i件物品
				else//选择放第i件物品
					c[i][j]=c[i-1][ j-w[i]]+p[i];
			}
			else//第i件物品放不下
			{
				c[i][j]=c[i-1][j];
			}
		}
	}
}

void printSelectPack(void)
{
	int i,j;
	int km=m;
	rm=0;
	printf("包最大载重为:%d, %d个物品的重量和价值分别为:",m,n);
	printf("\n物品:");
	for(i=1;i<=n;i++)
		printf("%3d",i);
	printf("\n重量:");
	for(i=1;i<=n;i++)
		printf("%3d",w[i]);
	printf("\n价值:");
	for(i=1;i<=n;i++)
		printf("%3d",p[i]);
	printf("\n\n被选中的包为:\n");
	for(i=n;i>0;i--)
		if(c[i][km]>c[i-1][km])
		{//如果从前i个物品选重量不超过m的物品价值大于从i-1个物品的价值，说明第i个物品被选中。
			sel[i]=true;
			km-=w[i];
			rm+=w[i];
		}
	for(i=1;i<=n;i++)
		if(sel[i]==true)
			printf("%3d",i);
	printf("\n包装下的重量为:%d,最大价值为:%d\n",rm,c[n][m]);
}

int main()
{
	init();
	pack();
	printSelectPack();
//	system("pause");
	return 0;
}
