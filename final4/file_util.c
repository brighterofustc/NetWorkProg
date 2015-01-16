/********************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                */
/*                                                                  */
/*  FILE NAME             :  server.c                               */
/*  PRINCIPAL AUTHOR      :  ZhangFuqiang                           */
/*  SUBSYSTEM NAME        :  server                                 */
/*  MODULE NAME           :  server                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2015/01/06                              */
/*  DESCRIPTION           :  a server                               */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by ZhangFuqiang,2014/01/14
 *
 */

#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

#include"trans_util.h"
#include"file_util.h"

/* get file size*/
int getfilesize(char *filename)
{
    FILE *fp ;   
    int size = -1;    

    if((fp = fopen(filename,"r"))== NULL)
    {
        printf("File: %s Open Error\n",filename);
    }  

    fseek(fp,0L,SEEK_END);
    size = ftell(fp);
    fclose(fp);

    return size;
}

/* send file request*/
void filerequestsend(int fd,char *filename)
{
    send(fd,filename,strlen(filename),0);
}

/* receive file request*/
int filerequestrecv(int fd,char *filename)
{
    int clientfd;
    struct sockaddr_in clientaddr;
    int len = sizeof(clientaddr);
    int n;

    listen(fd,10);

    clientfd = accept(fd, (struct sockaddr*)&clientaddr, &len);
    printf("cc %d\n",clientfd);
   
    if((n = recv(clientfd,filename,FILE_NAME_MAX_SIZE,0)) < 0)
    {
        printf("recv filename error\n");
        return -1;
    }
    filename[n] = '\0';

    return clientfd;
}



