/*
Question 1. -e lists all the processes that are running.
-l provides more information on the processes that are shown.
Question 2. --
Question 3. 2 times. Once for the parent, once for the child.
Question 4. Displays the processes running on the system as a tree that branches off to their respective children.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int  main(void){
        //int fd[2],fd2[2],  nbytes,nbytes2;
        pid_t childpid;
        char string [] = "We love 3SH3!\n";
        char parentString [80];
        char readbuffer [80],readbuffer2 [80];
        int childBuf ;
        int i=0,value=0,flag=0;
        char *end;
		
        int fd,ch,numberOfLines,newLineFlag,filePointer,j,avail;
		char stringOut[1];
		FILE *fp;
		struct stat mystat;
		void *pmap;
		int userValue1,userValue2;
       // pipe(fd);
        //pipe(fd2);
		
///////////SECTION THAT READS AND MAPS THE FILE TO MEMORY.
		fd=open("res.txt",O_RDWR);
			if(fd==-1)
				{ 
				perror("open");
				exit(1);
				}
			if (fstat(fd,&mystat)<0)
			{
				perror("fstat");
				close(fd);
				exit(1);
			
			}
			
			pmap=mmap(0,mystat.st_size,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);
			if(pmap==MAP_FAILED){
				perror("mmap");
				close(fd);
				exit(1);
				
			}
///////////////////////////////////////////////////////////////////


        if(( childpid = fork()) ==  -1){
                perror ("fork");
                exit (0);
        }

        if(childpid  == 0){
            printf("CHILD:Child is alive.\n");
			int pageSize;
			int memRegion;
                        size_t minAllo=getpagesize();
                        int test=0;
			unsigned char *vector;
            while(1){
					pageSize=getpagesize();
                                        const size_t ps = pageSize;
                                        printf("CHILD: page size is: %d\n",pageSize);
					vector=calloc(1,(mystat.st_size+pageSize-1)/pageSize);
					while((int)(*(char *)(pmap+test))!=0)
					{       value=strtol((char *)(pmap+test),NULL,10);
						avail=strtol((char *)(pmap+test+2),NULL,10);
						printf("CHILD: Resource: %d has %d.\n",value,avail);
					   test=test+5; //Plus 5 gets us to the next line.
					};
					mincore(pmap,mystat.st_size,vector);
                                        //memRegion=1;
                                        for( minAllo=0;minAllo<=mystat.st_size/pageSize;minAllo++){
                                        //printf("CHILD: Current status of pages: %lu\n",(unsigned long int)minAllo);
                                        if(vector[minAllo]&1){
                                            printf("CHILD: Current status of pages: %lu\n",(unsigned long int)minAllo);
                                        }
                                        free(vector);
                                        }
                                        test=0;
                                        sleep(10);
            }
            exit (0);
        }

        else{ //Parent
			while(1){
					printf("PARENT: What is the resource number to be remapped?\n");
					scanf("%d",&userValue1);
					printf("PARENT: What is the number of resources to be remapped/added?\n");
					scanf("%d",&userValue2);
					printf("PARENT: You entered: %d %d \n",userValue1,userValue2);
					newLineFlag=1;
					ch = 1;
					j=0;
					while(((int)(*(char *)(pmap+j)))!=0)
					{   value=strtol((char *)(pmap+j),NULL,10);
						//printf("Current value is: %d\n",value);
						if (value==userValue1){
						printf("PARENT: Found the resource.\n");
						avail=strtol((char *)(pmap+j+2),NULL,10);
						printf("PARENT: The number of available resources for this type is %d\n",avail);
						if(1)
							{
								printf("PARENT: Successfully remapped the number of resources to %d\n",userValue2+avail);
								sprintf(stringOut,"%d",(userValue2+avail));
								strncpy(pmap+j+2,stringOut,1);
								break;
							}
						
						else{
							printf("PARENT: Sorry, there are not enough resources to fulfill your request.\n");
							break;
							}
						}
						
					   j=j+5; //Plus 5 gets us to the next line.
					};
				msync(pmap,mystat.st_size,fd);

			}
        }

        return  0;
}
