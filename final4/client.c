/********************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                */
/*                                                                  */
/*  FILE NAME             :  server.c                               */
/*  PRINCIPAL AUTHOR      :  ZhangFuqiang                           */
/*  SUBSYSTEM NAME        :  server                                 */
/*  MODULE NAME           :  server                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2015/01/14                             */
/*  DESCRIPTION           :  a server                               */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by ZhangFuqiang,2014/01/14
 *
 */

#include<pthread.h>
#include<netinet/in.h> 
#include<sys/types.h>  
#include<sys/socket.h>  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<time.h>

#include"trans_util.h"
#include"file_util.h"

#define DEFAULT_PORT 8899


void *thread_serve(void *t_info)
{
    struct thread_info *tinfo;
    tinfo = (struct thread_info *)t_info;

    int clientfd = tinfo->socketfd;
    char *filename = tinfo->filename;
    char buffer[TRANS_SIZE];
    
    memset(buffer,'\0',sizeof(buffer)); 
    

    FILE *fp;

    if((fp = fopen(filename,"r+")) == NULL)
    {
        printf("file is not exist!\n");
        return;
    } 
    
    int i = 0;
    while(1)
    {
        struct trans_packet *tpack;
        tpack = (struct trans_packet*)malloc(sizeof(struct trans_packet));
    
        int n; 
        n = recv(clientfd,buffer,sizeof(buffer),0);
        memcpy(tpack,buffer,sizeof(*tpack));

        printf("aaa1 %s\n",filename); 
        printf("n = %d bytes,%d,%d\n",n,tpack->seekpos,tpack->size);  
  
        fseek(fp,tpack->seekpos,SEEK_SET);

        fwrite(tpack->buffer, sizeof(char), tpack->size, fp);

        if(tpack->size < MAXSIZE)
        {
            break;   
        }  
    }

    send(clientfd,"ACK",10,0);

    closefd(clientfd);
    fclose(fp);
}

void file_recv(int clientfd,char *filename,char*savefilename,int port)
{
    FILE *fp;
    fp = fopen(savefilename, "w+");
    close(fp);
    
    struct trans_packet tpacket;
    struct sockaddr_in clientaddr;
    int thread_num,n;

    n = recv(clientfd,(char*)&thread_num,4,0);

    printf("n %d Thread_num %d\n",n,thread_num);

    pthread_t *ptd;
    ptd = (pthread_t *)malloc(thread_num*sizeof(pthread_t));
    
    int i;
    for(i = 0 ; i < thread_num ; i ++)
    {
       
        struct thread_info *tinfo;
   
        tinfo = (struct thread_info*)malloc(sizeof(struct thread_info));

        int newclientfd = start_client(port);

        strcpy(tinfo->filename,savefilename); 
        tinfo->seekpos = -1;
        tinfo->socketfd = newclientfd;
        tinfo->size = -1;
      
        if(pthread_create((ptd+i),NULL,thread_serve,(void*)tinfo) != 0)
        {
             printf("create thread %d ERROR\n",i);
        }
    }
    
    for(i = 0; i < thread_num; i++)
    {
        pthread_join(ptd[i], NULL);
    } 
}

int main(int argc, char **argv) 
{ 
    char filename[FILE_NAME_MAX_SIZE],savefilename[FILE_NAME_MAX_SIZE];
    int serverfd,clientfd;
    int port = DEFAULT_PORT;
    time_t t_start,t_end; 

    if(argc > 1)  
    {
        port = atoi(argv[1]);
    }
    
    printf("Get file name:  ");
    scanf("%s",filename);

    printf("savefilename:  ");
    scanf("%s",savefilename);

    t_start=time(NULL);
    
    clientfd = start_client(port); 
   
    filerequestsend(clientfd,filename);

    file_recv(clientfd,filename,savefilename,port);
    
    close(serverfd); 

    t_end=time(NULL);
    printf("time : %.0fs\n",difftime(t_end,t_start));

    return 0; 
} 
