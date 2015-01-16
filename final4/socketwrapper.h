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

#ifndef _SOCKET_WRAPER_H_
#define _SOCKET_WRAPER_H_

/* start server*/
int start_server(int port);

/* start client*/
int start_client(int port);

/* close connect handler*/
void closefd(int fd);

#endif
