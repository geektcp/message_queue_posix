/*
 * ========================================================
 *
 *       Filename:  posix消息队列  消费者
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年6月12日 凌晨3时11分
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Haiyang Tang (http://geektcp.com), admin@geektcp.com
 *
 * =======================================================
 */
#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE       256      //定义buf大小
#define BUF_SIZE      8192      //定义Msg大小

struct msg_type_s {
        int len;
        char buf[MAX_SIZE];
};


int main()
{
        /*消息队列 */
        mqd_t msgq_id;
        struct msg_type_s msg;
        unsigned int sender;
        struct mq_attr msgq_attr;

        unsigned int recv_size = BUF_SIZE;
        const char *file = "/test";

        /* mq_open() for opening an existing queue */
        msgq_id = mq_open(file, O_RDWR);
        if (msgq_id == (mqd_t) - 1) {
                perror("mq_open");
                exit(1);
        }

        /* getting the attributes from the queue        --  mq_getattr() */
        if (mq_getattr(msgq_id, &msgq_attr) == -1) {
                perror("mq_getattr");
                exit(1);
        }

        printf("        Queue: %d\n\
        stores at most %ld messages\n\
        large at most %ld bytes each\n\
        currently holds %ld messages\n",
        msgq_id, msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

        if (recv_size < msgq_attr.mq_msgsize) { recv_size = msgq_attr.mq_msgsize; }


        while (1)
        {
                msg.len = -1;
                memset(msg.buf, 0, MAX_SIZE);

                /*mq_receive() 从由描述符 mqdes 引用的队列时删除优先级最高的最老的消息，并把放置到 msg_ptr 的缓存区内。 */
                /*参数 msg_len 指定缓冲区 msg_ptr 的大小：它必须大于队列的 mq_msgsize 属性(参数 mq_getattr)。 */
                /*如果 prio 不是 NULL，那么它指向的内存用于返回收到消息相关的优先级。 */
                if (mq_receive(msgq_id, (char *)&msg, recv_size, &sender) == -1) {
                        perror("mq_receive");
                        exit(1);
                }

                printf("msg.len = %d, msg.buf = %s\n", msg.len, msg.buf);
                //sleep(2);
        }

        if (mq_close(msgq_id) == -1) {
                perror("mq_close");
                exit(1);
        }

        return 0;
}