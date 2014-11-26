/*
time:2014-5-10
author:zhyq
description: 贪心法关于部分背包问题 
*/

/*
按照物品的 价重比，从高到底依次排序，优先装价重比高的物品
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<assert.h>

#ifndef MY_DEFINE
#define MY_DEFINE
#define false 0
#define true 1
#define WSIZE 10//重量的范围
#define PSIZE 10//物品价值的范围
#endif

typedef struct goods_
{
	int id;//物品id号
	int p;//指向物品价值
	int w;//指向物品重量
	float sel;//标记物品是否被选中
}goods;
goods *g=NULL;
int n;//物品的件数
int m;//包能放下的最大重量
float rp;//real p 最后包装下的总价值
int rm;//real m 最后包装下的总重量
void init(void);//init
void pack(void);
void printSelectPack(void);
inline float getPPW(goods gs);//算价重比
void goodsSort(goods *gs);//把物品按价重比从高到底排序
void goodsPrint(void);//打印物品信息
inline float getPPW(goods gs)
{
	assert(gs.w>0);
	return (float)gs.p/(float)gs.w;
}
void init(void)
{
	int i;
	srand(time(NULL));
	printf("请输入包的最大载重:\n");
	scanf("%d",&m);
	printf("请输入物品的件数:\n");
	scanf("%d",&n);
	g=(goods *)malloc(sizeof(goods) * (n+1));
	if(g==NULL)
	{	
		printf("分配内存失败\n");
		system("pause");
		exit(0);
	}
	for(i=0;i<=n;i++)
	{	
		g[i].id=i;
		g[i].sel=0;
	}
	for(i=1;i<=n;i++)
		g[i].p=random()%PSIZE+1;//scanf("%d",&p[i]);
	for(i=1;i<=n;i++)
		g[i].w=random()%WSIZE+1;//scanf("%d",&w[i]);
	rm=0;
	rp=0;
	return ;
}
void goodsSort(goods* gs)
{
	//按 价重比 排序
	goods temp;
	int i,j;
	for(i=1;i<=n;i++)
		for(j=i+1;j<=n;j++)
		{
			//p per w 每个物品的重量单位的价值 （价重比）
			if(getPPW(gs[j]) > getPPW(gs[i]) )
			{
				temp=gs[i];gs[i]=gs[j];gs[j]=temp;
			}
		}	

}

void pack(void)
{
	int i;
	printf("物品价重比排序前：\n");
	goodsPrint();
	goodsSort(g);
	printf("物品价重比排序后：\n");
	goodsPrint();	
	for(i=1;i<=n;i++)
	{
		if(rm<m)//如果包实际装下的重量小于最大重量
		{
			if( (rm + g[i].w) <= m )//如果第i个物品完全可以装下，则整个装下
			{
				rm+=g[i].w;//更新背包重量
				rp+=(float)g[i].p;//更新背包价值
				g[i].sel=1;//标记背包被整个装下
			}
			else //只能装下部分i物品
			{
				//g[i].sel记录能装下此物品的份数
				g[i].sel=((float)m - (float)rm) / g[i].w;
				rm=m;//记录装满
				rp+=(float)g[i].p*g[i].sel;
				return ;
			}
		}
		else//如果装不下东西，则返回
		{
			return;
		}
	}	
}	
void goodsPrint(void)
{
	int i,j;
	int km=m;
	rm=0;
	//printf("包最大载重为:%d, %d个物品的重量和价值分别为:",m,n);
	printf("物品:");
	for(i=1;i<=n;i++)
		printf("%3d",g[i].id);
	printf("\n重量:");
	for(i=1;i<=n;i++)
		printf("%3d",g[i].w);
	printf("\n价值:");
	for(i=1;i<=n;i++)
		printf("%3d",g[i].p);
	printf("\n");

}

void printSelectPack(void)
{
	int i;
	printf("被选中的物品为：\n选中物品:");
	for(i=1;i<=n;i++)
		if(g[i].sel>0)
		{
			printf("%4d  ",g[i].id);
		}
	printf("\n所占分值:");
	for(i=1;i<=n;i++)
		if(g[i].sel>0)
			printf("%.2lf  ",g[i].sel);
	printf("\n总价值为：%.2lf\n  ",rp);
}

int main()
{
	init();
	pack();
	printSelectPack();
//	system("pause");
	return 0;
}
