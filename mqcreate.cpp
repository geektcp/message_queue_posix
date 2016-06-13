/*
 * ========================================================
 *
 *       Filename:  posix消息队列  生产者
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年6月12日 凌晨3时11分
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Haiyang Tang (http://geektcp.com), admin@geektcp.com
 *          usage:  gcc -lrt -o mqcreate test_mqcreate.c && ./mqcreate
 * =======================================================
 */

#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

#define MAXSIZE     256   //定义buf大小
#define BUFFER      8192 //定义Msg大小

struct msg_type_s {
        int len;
        char buf[MAXSIZE];
};

int main(int argc, char **argv)
{
        int c, flags;
        mqd_t msgq_id;
        struct mq_attr msgq_attr;
        struct msg_type_s msg;
        unsigned int prio;
        //char *queue_path = "/tmp/mq/test";
        char *queue_path = "/test";

        flags = O_RDWR | O_CREAT;
        prio  = 1;

        //mq_open包含了创建队列的功能，如果之前已经有了同名的消息队列，再用mq_open会阻塞，程序无法执行。
        //解决办法是删除那个同名队列
        //但是如果另外一个程序正常运行并接收这个同名队列，那么删除后重新创建同名队列后，接收程序无法收到消息，要重启才能收到
        //mq_unlink(queue_path);
        msgq_id = mq_open(queue_path,  O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL);
        if(msgq_id<0) {
                perror("mq_open");
                exit(1);
        }

        mq_getattr(msgq_id, &msgq_attr);

        printf("        Queue: %d\n\
        stores at most %ld messages\n\
        large at most %ld bytes each\n\
        currently holds %ld messages\n",
        msgq_id, msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

        mq_setattr(msgq_id, &msgq_attr, NULL);

        int i = 0;
        while(i < 1)
        {
                //printf("start...\n");
                msg.len = i;
                memset(msg.buf, 0, MAXSIZE);
                //sprintf(msg.buf, "the seq: %d---\n", i);
                //sprintf(msg.buf, "ifconfig");
                sprintf(msg.buf, "df -h");


                printf("msg.len = %d, msg.buf = %s\n", msg.len, msg.buf);

                /*  int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);  */
                /*mq_send() 把 msg_ptr 指向的消息加入由 mqdes 引用的消息队列里。*/
                /*参数 msg_len 指定消息 msg_ptr 的长度：这个长度必须小于或等于队列 mq_msgsize 属性的值。零长度的消息是允许。*/
                if(mq_send(msgq_id, (char*)&msg, sizeof(struct msg_type_s), prio) == -1)
                {
                    perror("mq_send");
                    exit(1);
                }

                i++;
                sleep(1);
        }

        mq_close(msgq_id);

        exit(0);
}