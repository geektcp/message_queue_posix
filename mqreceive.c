/*
 * ========================================================
 *
 *       Filename:  posix��Ϣ����  ������
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016��6��12�� �賿3ʱ11��
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

#define MAX_SIZE       256      //����buf��С
#define BUF_SIZE      8192      //����Msg��С

struct msg_type_s {
        int len;
        char buf[MAX_SIZE];
};


int main()
{
        /*��Ϣ���� */
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

                /*mq_receive() ���������� mqdes ���õĶ���ʱɾ�����ȼ���ߵ����ϵ���Ϣ�����ѷ��õ� msg_ptr �Ļ������ڡ� */
                /*���� msg_len ָ�������� msg_ptr �Ĵ�С����������ڶ��е� mq_msgsize ����(���� mq_getattr)�� */
                /*��� prio ���� NULL����ô��ָ����ڴ����ڷ����յ���Ϣ��ص����ȼ��� */
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