#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/ioctl.h>

int fd = 0;

void sigio_signal_func(int arg)
{
        int key_value = 0;
        int ret = 0;

        ret = read(fd, &key_value, sizeof(key_value));
        if (ret < 0) {
                printf("read error!\n");
                goto error;
        } else {
                printf("key_vlaue = %d\n", key_value);
        }
error:
        return;
}

int main(int argc, char *argv[])
{
        int ret = 0;
        int arg = 0, cmd = 0;
        char *filename = NULL;
        unsigned char str[10] = {0};
        int flags = 0;

        if (argc != 2) {
                printf("Error Usage!\n"
                       "Usage %s filename 0:1\n"
                       ,argv[0]);
                ret = -1;
                goto error;
        }

        filename = argv[1];
        fd = open(filename, O_RDWR);
        if (fd == -1) {
                perror("open failed!\n");
                ret = -1;
                goto error;
        }

        signal(SIGIO, sigio_signal_func);       /* 设置信号SIGIO的处理函数 */
        fcntl(fd, F_SETOWN, getpid());          /* 设置当前的进程状态 */
        flags = fcntl(fd, F_GETFD);
        fcntl(fd, F_SETFL, flags | FASYNC);     /* 设置进程启动异步通知功能 */

        while (1) {
              sleep(20);
        }

error:
        close(fd);
        return ret;
}
