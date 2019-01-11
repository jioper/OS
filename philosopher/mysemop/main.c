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

/*
	哲学家进餐
*/
void eat(int i) //哲学家吃饭
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

/*
	哲学家进屋
*/
void philosopher(int i) 
{
int a=0;
   srand((int)time(0));
   while (1)
   {
for(a=0;a<i;a++)
printf("\t\t\t\t\t");
      Psem(room); //哲学家申请进屋

      printf("Philosopher %d entered the room.\n", i);

      eat(i);

      Vsem(room); //哲学家出屋子
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

   //只有四个哲学家可以进屋，算法1
   room = CreateSem(4);
   for (i = 0; i < N; i++)
      forks[i] = CreateSem(1);

   // 五个筷子
   for (i = 0; i < N; i++)
   {
      pid = fork();
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
	输入a终止
*/
   while (getchar() != 'a');
   for (i = 0; i < N; ++i)
   {
          kill(child_ids[i], SIGTERM); 

          DeleteSem(forks[i]);
   }

   DeleteSem(room);
   return 0;
}
