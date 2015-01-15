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

#ifndef _TRANS_UTIL_H_
#define _TRANS_UTIL_H_

#define MAXSIZE 1024*80
#define FILE_NAME_MAX_SIZE 512 

typedef struct trans_packet
{
    int seekpos;
    char buffer[MAXSIZE];
    int size;
}*transinfo; 

typedef struct thread_info
{
     char filename[FILE_NAME_MAX_SIZE];
     int seekpos;
     int socketfd;
     int size;
}*threadinfo;

#endif
