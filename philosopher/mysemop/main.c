#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#include "mysemop.h"

#define N 5


int forks[5];
int room;

// 哲学家进餐
void eat(int i) /*i：哲学家编号，从0 到4*/
{
int a=0;
   int t = rand() % 5 + 1;
for(a=0;a<i;a++)
printf("\t\t\t\t\t");
   Psem(forks[i]);
   printf("Philosopher %d picked up the left chopstick. \n", i);
   Psem(forks[(i + 1) % N]);
for(a=0;a<i;a++)
printf("\t\t\t\t\t");
   printf("Philosopher %d picked up the right chopstick. \n", i);
for(a=0;a<i;a++)
printf("\t\t\t\t\t");
   printf("Philosopher %d is eating on %d secs. \n", i, t);
   sleep(t);
   Vsem(forks[i]);
for(a=0;a<i;a++)
printf("\t\t\t\t\t");
   printf("Philosopher %d put down the left chopstick. \n", i);
   Vsem(forks[(i + 1) % N]);
for(a=0;a<i;a++)
printf("\t\t\t\t\t");
   printf("Philosopher %d put down the right chopstick. \n", i);
for(a=0;a<i;a++)
printf("\t\t\t\t\t");
   printf("Philosopher %d has finished eating.\n", i);
}


void philosopher(int i) /*i：哲学家编号，从0 到4*/
{
int a=0;
   srand((int)time(0));
   while (1)
   {
for(a=0;a<i;a++)
printf("\t\t\t\t\t");
      Psem(room); //每进来一个哲学家，对room进行一次P操作。因为room初始为4,所以第五个哲学家会休眠。

      printf("Philosopher %d entered the room.\n", i);

      eat(i);

      Vsem(room); //哲学家进完餐。
for(a=0;a<i;a++)
printf("\t\t\t\t\t");
      printf("Philosopher %d left the room.\n", i);
   }
}

int main(void)
{  
   int i;
   pid_t pid;
   char kill_cmd[100];
   int child_ids[N];

   //room初始为4.
   room = CreateSem(4);
   for (i = 0; i < N; i++)
      forks[i] = CreateSem(1);

   // 创建5个筷子的信号量。
   for (i = 0; i < N; i++)
   {
      pid = fork();

       /*
        pid==0 说明在子进程里面。
        */
      if (pid == 0)
      {
         philosopher(i);
      }
      else
      {
         child_ids[i] = pid;
      }
   }

    /*
     输入q即可停止杀死进程，停止程序。
     */
   while (getchar() != 'q');
   for (i = 0; i < N; ++i)
   {
          kill(child_ids[i], SIGTERM); 

          DeleteSem(forks[i]);
   }

   DeleteSem(room);
   return 0;
}
