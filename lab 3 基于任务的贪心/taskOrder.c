/*
  author:zhyq
  class:算法导论一班
  time:2014-6-12
  description:实现在单处理器上有期限有惩罚的单位任务调度，使得惩罚代价最小（见算法导论第二版P239）
              首先把输入的任务存于my_task中，然后将my_task中的任务一个个加入独立子集中测试，如果还满足独立
              子集，说明此时加入的任务是早任务，否则此任务是迟任务。接着测试下一个任务。
              遍历的时间为O（n）,每次遍历时测试独立子集时间复杂度为O（n）,所以总的时间复杂度为：O（n*n）.
              
  使用说明：先输入一个整数表示n 一个一共有多少个任务，然后依次输入每个任务的限制日期和罚款值，可以在运行时用
          管道从input文件中得到输入，input初始化为算法导论（第二版）P241上的例子
 */
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

//定义早任务为1 迟任务为2 所有任务为0 用于打印时记录类型
#define EARLYTASK 1
#define LATETASK 2
#define ALLTASK 0

//定义任务结构体
typedef struct task_
{
    int id;//任务ID号
    int d;//任务的截止时间
    int w;//超期限的惩罚值
    bool is_early;//标记是否为早任务
}task;

int num_of_task ;//总任务个数
task * my_task=NULL;//总任务指针
task * orderd_task=NULL;//临时保存task用
int early_task_num=0;//记录此前早任务数。

//交换两个任务
void swap(task &ta1, task &ta2)
{
    task temp;
    temp=ta1;ta1=ta2;ta2=temp;
}

//初始化 得到
void init(void)
{
    int i,j;
    early_task_num=0;
    printf("输入任务个数\n");
    scanf("%d",&num_of_task);
    my_task=(task *)malloc(sizeof(task)*(num_of_task + 1));
    orderd_task=(task *)malloc(sizeof(task)*(num_of_task + 1));    
    if(my_task==NULL || orderd_task==NULL)
    {
        printf("存储任务分配内存失败\n");
        system("pause");
        //exit -1;
    }
    printf("输入每个任务的任务截止时间核惩罚值\n");
    for(i=1;i<=num_of_task;i++)
    {
        my_task[i].id=i;
        my_task[i].is_early=false;
        scanf("%d %d",&my_task[i].d,&my_task[i].w);
    }
    
    for(i=1;i<=num_of_task;i++)
        orderd_task[i]=my_task[i];
    
    for(i=1;i<=num_of_task;i++)
        for(j=i+1;j<=num_of_task;j++)
            if(orderd_task[i].d>orderd_task[j].d)
                swap(orderd_task[i],orderd_task[j]);
}

//打印函数，使输出更直观
void print(task * ta,int type)
{
    int i,j;
    if(type==ALLTASK)
    {
        printf("\n任务号码：");
        for(i=1;i<=num_of_task;i++)
            printf("%4d",ta[i].id);
        printf("\n限制日期：");
        for(i=1;i<=num_of_task;i++)
            printf("%4d",ta[i].d);
        printf("\n罚款代价：");
        for(i=1;i<=num_of_task;i++)
            printf("%4d",ta[i].w);
    }
    else if(type==EARLYTASK)
    {
        printf("\n\n早任务号：");
        for(i=1;i<=num_of_task;i++)
            if(ta[i].is_early==true)
                printf("%4d",ta[i].id);
        printf("\n限制日期：");                    
        for(i=1;i<=num_of_task;i++)
            if(ta[i].is_early==true)
                printf("%4d",ta[i].d);
        printf("\n罚款代价：");                   
        for(i=1;i<=num_of_task;i++)
            if(ta[i].is_early==true)
                printf("%4d",ta[i].w);
            
    }
    else if(type==LATETASK)
    {

        printf("\n\n迟任务号：");
        for(i=1;i<=num_of_task;i++)
            if(ta[i].is_early==false)
                printf("%4d",ta[i].id);
        printf("\n限制日期："); 
        for(i=1;i<=num_of_task;i++)
            if(ta[i].is_early==false)
                printf("%4d",ta[i].d);
        printf("\n罚款代价："); 
        for(i=1;i<=num_of_task;i++)
            if(ta[i].is_early==false)
                printf("%4d",ta[i].w);       
    }

}

//独立子集，测试把第task_id号任务放入独立子集中。如果还是独立子集，则为早任务，否则为迟任务。
bool indepSet(int task_id)
{
    int i,orderd_task_index;
    int count=1;
    //找到 my_task任务中id号为task_id的任务在经过 对映 按限制日期排序后所在的位置orderd_task_id
    for(i=1;i<=num_of_task;i++)
    {
        if(orderd_task[i].id==task_id)
        {
            //暂且标记此时要测试的任务为早任务
            orderd_task_index=i;
            orderd_task[i].is_early=true;
            early_task_num++;
            my_task[task_id].is_early=true;
            break;
        }
    }
    //加入my_task[task_id]任务后，把这些任务按限制如期增序检查，看是否满足没有迟任务
    for(i=1;i<=num_of_task;i++)
    {
        if(orderd_task[i].is_early && orderd_task[i].d>=count)
        {
            count++;
        }
        else if(orderd_task[i].is_early && orderd_task[i].d<count && count<=early_task_num)
        {
            //如果有迟任务，则此测试的任务设置成迟任务
            orderd_task[orderd_task_index].is_early=false;
            my_task[task_id].is_early=false;
            early_task_num--;
            return false;
        }
    }
    return true;
}

/*
  
  从任务的前面开始扫描如果两个任务a[i],a[j](i>j)满足如下两个条件，需要交换a[i],a[j]：
  1 a[i],a[j] 都为早任务，且a[i].d<a[j].d (a[i]的截止日期大于a[j]),则交换a[i],a[j]
  2 a[i]为早任务，a[j]为迟任务，则交换a[i],a[j]
  由于已经按限制日期排过序（在init()函数中冒泡排序，并存于orderd_task中），所以没有在重复左交换
 */
int taskOrder()
{
    int i,j;
    int count=0;
    print(my_task,ALLTASK);

    //从i=1开始，依次把my_task[i]进行测试，看加入my_task[i]是否能保持独立集。
    for(i=1;i<=num_of_task;i++)
    {
        //todo 暂且把my_test[i]当作早任务，测试是否是独立集
        indepSet(my_task[i].id);//不满足独立集，把设成迟任务      
    }
   
    //打印早任务序号
    print(orderd_task,EARLYTASK);

    //打印迟任务序号
    print(orderd_task,LATETASK);

    //计算总共罚款值
    for(i=1;i<=num_of_task;i++)
        if(!orderd_task[i].is_early)
            count+=orderd_task[i].w;
    printf("\n\n总罚款最小为：%d\n",count);
}


int main(int argc, char *argv[])
{
    init();
    taskOrder();
    return 0;
}
