/* set ts=4
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<sys/mman.h>
#include	<malloc.h>
#include	<string.h>
#include	<sys/types.h>


int	main()
{  
	int	fd=-1,page_size=-1;
	pid_t	pid=-1; 

	int     dst_Char;

	
	
		dst_Char = 'W';

	
	page_size = getpagesize();
	printf("The size of memory page is %d Bytes!\n",page_size); 

	
                if ((fd=open("./f1",O_RDWR)) < 0) {
		fprintf(stderr,"The file f1 is not exist! Pls check it. Good Luck! Bye.\n");
		_exit(1);
	}
	
                
	char * src = (char *)mmap(NULL,page_size,PROT_WRITE,MAP_SHARED,fd,0);
	if(src==MAP_FAILED) {
		fprintf(stderr,"Error,Can not map memory.\n");
		_exit(1);
	}

	/**********************************************************************************
 	*	⑵显示src的内容;   
 	*	⑶使用malloc()函数申请一个页面大小的内存空间，地址返回到dst;
 	*	⑷使用memmove()函数将共享存储区的内容复制到dst;
 	*	⑸显示dst的内容;
	***********************************************************************************/

                printf("Now We are before malloc() function. src is:\n%s\n",src);

	
	char	*dst = 0;
	if (!(dst = malloc(page_size))) {
		fprintf(stderr,"Can't malloc memory to dst. Game over!\n");
		_exit(1);
	}

	memset(dst,0,page_size);    
	
	
      	
	bcopy(src,dst,page_size);
	
	
	printf("Now We are before malloc() function. dst is:\n%s\n",dst);

	
  	pid=fork();

	if(pid==0){ 
	/**********************************************************************************
		子进程使用memccpy()将文件f1中W之后内容的地址复制到temp，
		将temp中前2个字符用@替换。之后将修改后的dst的内容使用memmove()复制回src，
		关闭文件f1，释放dst。	
	**********************************************************************************/
		
	
		
	char	*temp = 0;

                if (!(temp = (char *)memccpy(dst, src, dst_Char,page_size))) {		
                        printf("\t\t\tWarnning! Can't find the char W in src. Game Over!\n");
                        free(dst);
                        dst = 0;
                        _exit(0);
		}
	
		
	printf("\t\t\tThe addr pointed by temp is 0x%x. Its contect is:\n%s\n",temp,strlen(temp)==0?"NULL":temp);

	

		temp[0] = temp[1] = '@';
                printf("\t\t\tAfter replacing with 2 @, the dst is:\n%s\n",dst);

	

		memmove(src,dst,strlen(dst));

	
		printf("\t\t\tPID=%d,PPID=%d: After move dst to src, the src is:\n%s\n",getpid(),getppid(),src);


             

		close(fd);
		free(dst);
		dst = 0;

		_exit(0);
	} else if (pid>0) {       
		
                               

		wait(NULL);

		/*****************************************************************************  
			⑿使用memset()将共享存储区src中前2个字符用QQ替换，并显示替换后的内容(包含PID及PPID);
			⒀显示经2次修改后的f1的最终内容;
		******************************************************************************/
		
	
	memset(src,'Q',2);

	}
		

	printf("PID is: %d,  PPID is: %d,  The src value of end is:\n%s\n\n", getpid(),getppid(),src);   
	
	if(munmap(src,page_size)==0) {    
		printf("PID is: %d,  PPID is: %d, munmap success\n\n", getpid(), getppid());
	} else {
		printf("PID is: %d,  PPID is: %d,  munmap failed\n\n",getpid(), getppid());
	}

	
	close(fd);
	free(dst);
}
