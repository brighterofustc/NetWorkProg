/********************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                */
/*                                                                  */
/*  FILE NAME             :  client.c                               */
/*  PRINCIPAL AUTHOR      :  ZhangFuqiang                           */
/*  SUBSYSTEM NAME        :  server                                 */
/*  MODULE NAME           :  server                                 */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2015/01/06                             */
/*  DESCRIPTION           :  a client                               */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by ZhangFuqiang,2014/01/06
 *
 */

#include<netinet/in.h>
#include<sys/types.h> 
#include<sys/socket.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 

#include"dbtime.h"

#define SERVER_PORT 8888 
#define BUFFER_SIZE 80*1024 
#define FILE_NAME_MAX_SIZE 512 
 
int main() 
{ 
    struct sockaddr_in client_addr; 
    bzero(&client_addr, sizeof(client_addr)); 
    client_addr.sin_family = AF_INET; 
    client_addr.sin_addr.s_addr = htons(INADDR_ANY); 
    client_addr.sin_port = htons(0); 
 
    int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
  
    if(client_socket_fd < 0) 
    { 
        perror("Create Socket ERROR\n"); 
        exit(1); 
    } 
 
    if(-1 == (bind(client_socket_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)))) 
    { 
        perror("Client Bind ERROR"); 
        exit(1); 
    } 
 
    struct sockaddr_in server_addr; 
  
    bzero(&server_addr, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET; 
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == 0) 
    { 
         perror("Server IP Address EEEOR"); 
         exit(1); 
    } 
  
    server_addr.sin_port = htons(SERVER_PORT); 
    socklen_t server_addr_length = sizeof(server_addr); 

    dbtime_startTest ("Connect & Recv");
 
    if(connect(client_socket_fd, (struct sockaddr*)&server_addr, server_addr_length) < 0) 
    { 
        perror("connect with server error"); 
        exit(0); 
    } 
 
    char file_name[FILE_NAME_MAX_SIZE+1]; 
    
    bzero(file_name, FILE_NAME_MAX_SIZE+1); 
    printf("Input File Name :  "); 
    scanf("%s", file_name); 

    char buffer[BUFFER_SIZE]; 
  
    bzero(buffer, BUFFER_SIZE); 
    strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE?BUFFER_SIZE:strlen(file_name)); 
 
    if(send(client_socket_fd, buffer, BUFFER_SIZE, 0) < 0) 
    { 
        perror("Send File Name Failed:"); 
        exit(1); 
    } 
  
    FILE *fp = fopen(file_name, "w"); 
    if(NULL == fp) 
    { 
        printf("File:\t%s Can Not Open To Write\n", file_name); 
        exit(1); 
    } 
 
    bzero(buffer, BUFFER_SIZE); 
  
    int length = 0; 
    while((length = recv(client_socket_fd, buffer, BUFFER_SIZE, 0)) > 0) 
    { 
         if(fwrite(buffer, sizeof(char), length, fp) < length) 
         { 
              printf("File: %s Write Failed\n", file_name); 
              break; 
         } 
         bzero(buffer, BUFFER_SIZE); 
     } 

    dbtime_endAndShow ();

    printf("Receive File: %s Finished!\n", file_name); 
 
    dbtime_startTest ("Sleep 5s");
    sleep(5);
    dbtime_endAndShow ();
    dbtime_finalize (); 

    close(fp); 
    close(client_socket_fd); 
    return 0; 
} 
