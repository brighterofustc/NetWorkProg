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

#include"trans_util.h"
#include"file_util.h"

#define DEFAULT_PORT 8899


void *thread_serve(void *t_info)
{
    struct thread_info *tinfo;
    tinfo = (struct thread_info *)t_info;

    int clientfd = tinfo->socketfd;
    char *filename = tinfo->filename;

    FILE *fp;

    if((fp = fopen(filename,"r+")) == NULL)
    {
        printf("file is not exist!\n");
        return;
    } 
    
    struct trans_packet *tpack;
    tpack = (struct trans_packet*)malloc(sizeof(struct trans_packet));

    recv(clientfd,(char*)&tpack,sizeof(tpack),0);
    
    fseek(fp,tpack->seekpos,SEEK_SET);

    fwrite(tpack->buffer, sizeof(char), tpack->size, fp);

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
    int thread_num;

    recv(clientfd,(char*)&thread_num,4,0);
    printf("Thread_num %d",thread_num);

    pthread_t *ptd;
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

        if(pthread_create((ptd+i),NULL,thread_serve,(void*)tinfo)!=0)
        {
             printf("create thread %d ERROR\n",i);
        }
    }
     
}

int main(int argc, char **argv) 
{ 
    char *filename,*savefilename;
    int serverfd,clientfd;
    int port = DEFAULT_PORT;
    
    if(argc > 1)  
    {
        port = atoi(argv[1]);
    }
    
  
    while(1)
    {
        printf("Get file name:  ");
        scanf("%s",filename);

        printf("savefilename:  ");
        scanf("%s",savefilename);

        clientfd = start_client(port); 

        filerequestsend(clientfd,filename);
        file_recv(clientfd,filename,savefilename,port);
    }
    
    close(serverfd); 
    return 0; 
} 
