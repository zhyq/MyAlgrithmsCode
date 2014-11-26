/*

time:2014-5-10
author:zhyq
description: contrast the costed time for sorting  two different array 
*/

#include<stdio.h>
#include<linux/types.h>
#include<time.h>
#define SIZE 100

int arrayOrderd[SIZE];
int arrayRandom[SIZE];
void init(void);
void callSort(int *);
void Qsrot(int *,int ,int);
__u64 rdtsc(void);

void init(void)
{
	srand(time(NULL));
	int i;
	for(i=0;i<SIZE;i++)
		arrayOrderd[i]=SIZE-i;
	for(i=0;i<SIZE;i++)
		arrayRandom[i]=random()%SIZE;
}

__u64 rdtsc(void)
{
	__u32 hi,lo;
	__asm__ __volatile__
	(
        "rdtsc"
	:"=d"(hi),"=a"(lo)
	);
	return (__u64)hi<<32|lo;
	
}

void Qsort(int *array,int low,int high)
{
        if(low>=high)
        return ;
        int i,j;
        int tempLow=low;
        int tempHigh=high;
        int temp;
        temp=array[low];//get the low
        while(tempLow<tempHigh)
        {
                while(array[tempHigh]>=temp && tempHigh>tempLow) tempHigh--;
                    array[tempLow]=array[tempHigh];
                while(array[tempLow]<=temp && tempLow<tempHigh) tempLow++;
                    array[tempHigh]=array[tempLow];
        }
        array[tempLow]=temp;
        Qsort(array,low,tempLow-1);
        Qsort(array,tempHigh+1,high);
}


void callSort(int* array)
{
	int i;
	__u64 begin,end;
	printf("Before sort, array as:\n");
	for(i=0;i<SIZE-1;i++)
		printf("%d ",array[i]);printf("%d",array[SIZE-1]);
	begin=rdtsc();//recode the time

	Qsort(array,0,SIZE-1);//sorting...

	end= rdtsc();//recode the time
	printf("\nAfter sort, array as:\n");
	for(i=0;i<SIZE-1;i++)
		printf("%d ",array[i]);printf("%d",array[SIZE-1]);
	printf("\nsorting cost cycles is %llu\n",end-begin);
}


int main(void)
{
	int i;
	init();
	printf("****call sort for ordere array***\n");
	callSort(arrayOrderd);
        printf("\n****call sort for random array***\n");
	callSort(&arrayRandom[0]);
}
