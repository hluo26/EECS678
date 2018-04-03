#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
typedef struct
{
  int pid;
  int atime;
  int prior;
}info;

int ascending_order(const void *a,const void *b)
{
  return ( ((const info*)a)->atime - ((const info*)b)->atime);
}

int descending_order(const void *a,const void *b)
{
  if(((const info*)b)->prior==((const info*)a)->prior)
  {
    return ( ((const info*)a)->atime - ((const info*)b)->atime );
  }
  else
  {
    return ( ((const info*)b)->prior - ((const info*)a)->prior );
  }

}

int main()
{
  info proc[7];
  int ipid[7]={5,2,7,3,1,4,6};
  int itime[7]={10,4,14,6,2,8,12};
  int iprior[7]={3,0,0,1,1,2,3};

  for(int i=0;i<7;i++)
  {
    proc[i].pid=ipid[i];
    proc[i].atime=itime[i];
    proc[i].prior=iprior[i];
  }
  printf("\nQuicksorting for arrival time in ascending order... \n");
  qsort(proc,7,sizeof(info),ascending_order);

   printf("\nAfter sorting the arrival time is: \n");
  for(int i=0;i<7;i++)
  {
    printf("%d ", proc[i].atime);
  }
  printf("\nAfter sorting the prior is: \n");
  for(int i=0;i<7;i++)
  {
    printf("%d ", proc[i].prior);
  }
  printf("\nQuicksorting for priority in descending order... \n");
  qsort(proc,7,sizeof(info),descending_order);
  printf("\nAfter sorting the prior is: \n");
  for(int i=0;i<7;i++)
  {
    printf("%d ", proc[i].prior);
  }
  printf("\nAfter sorting the arrival time is: \n");
  for(int i=0;i<7;i++)
  {
    printf("%d ", proc[i].atime);
  }

}
