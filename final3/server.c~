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
    int seekpos = tinfo->seekpos;
    char *filename = tinfo->filename;
    int size = tinfo->size;

    FILE *fp;

    if((fp = fopen(filename,"r")) == NULL)
    {
        printf("file is not exist!\n");
        return;
    } 
    
    fseek(fp,seekpos,SEEK_SET);
    
    struct trans_packet *tpack;
    tpack = (struct trans_packet*)malloc(sizeof(struct trans_packet));

    if(fread(tpack->buffer,sizeof(char),size,fp) > 0)
    {
        tpack->seekpos = seekpos;
        tpack->size = size;
        send(clientfd,(char*)tpack,sizeof(*tpack),0);        
    }
    else
    {
        printf("over!\n");
    }
     
    char resp[10];
    recv(clientfd,resp,10,0);
    if(!strcmp(resp,"ACK"))
    {
        printf("%d send sucess!\n",seekpos);
    }

    closefd(clientfd);
    fclose(fp);
}

void file_send(int clientfd,int serverfd,char *filename)
{
    struct trans_packet tpacket;
    struct sockaddr_in clientaddr;
    int thread_num;
    int filesize;

    filesize = getfilesize(filename);
    printf("filesize : %d\n",filesize);

    thread_num = filesize/MAXSIZE + 1;


    send(clientfd,(char*)&thread_num,4,0);

    pthread_t *ptd;
    int i,size = 0;
    for(i = 0 ; i < thread_num ; i ++)
    {
        listen(serverfd,100);
        struct thread_info *tinfo;
   
        tinfo = (struct thread_info*)malloc(sizeof(struct thread_info));

        int newclientfd = accept(serverfd, (struct sockaddr*)&clientaddr,&size);

        strcpy(tinfo->filename,filename); 
        tinfo->seekpos = i*MAXSIZE; 
        tinfo->socketfd = newclientfd;

        if((i+1)*MAXSIZE > filesize)
        {
            tinfo->size = filesize - i * MAXSIZE;
        } 
        else
        {
            tinfo->size = MAXSIZE; 
        }

        if(pthread_create((ptd+i),NULL,thread_serve,(void*)tinfo)!=0)
        {
             printf("create thread %d ERROR\n",i);
        }
    }
     
}

int main(int argc, char **argv) 
{ 
    char *filename;
    int serverfd,clientfd;
    int port = DEFAULT_PORT;

    if(argc > 1)  
    {
        port = atoi(argv[1]);
    }
    
    serverfd = start_server(port); 
  
    if(serverfd < 0) 
    { 
        perror("Create Socket ERROR"); 
        exit(1); 
    } 
  
    while(1)
    {
        clientfd = filerequestrecv(serverfd,filename);

        file_send(clientfd,serverfd,filename);
    }
    
    close(serverfd); 
    return 0; 
} 
