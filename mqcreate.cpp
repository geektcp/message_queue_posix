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
 *          usage:  gcc -lrt -o mqcreate test_mqcreate.c && ./mqcreate
 * =======================================================
 */

#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

#define MAXSIZE     256   //����buf��С
#define BUFFER      8192 //����Msg��С

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

        //mq_open�����˴������еĹ��ܣ����֮ǰ�Ѿ�����ͬ������Ϣ���У�����mq_open�������������޷�ִ�С�
        //����취��ɾ���Ǹ�ͬ������
        //�����������һ�������������в��������ͬ�����У���ôɾ�������´���ͬ�����к󣬽��ճ����޷��յ���Ϣ��Ҫ���������յ�
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
                /*mq_send() �� msg_ptr ָ�����Ϣ������ mqdes ���õ���Ϣ�����*/
                /*���� msg_len ָ����Ϣ msg_ptr �ĳ��ȣ�������ȱ���С�ڻ���ڶ��� mq_msgsize ���Ե�ֵ���㳤�ȵ���Ϣ������*/
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