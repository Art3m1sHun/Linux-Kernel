#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define LED_ON   _IOW('a', 1, int32_t *)
#define LED_OFF  _IOW('a', 0, int32_t *)

#define GPIO_PATH "/dev/gpio_device"

int fd, opt;
int number, foo;
char write_buff[1024];
char read_buff[1024];

void printMENU()
{
    printf("===================\n");
    printf("    1.  LED ON  \n");
    printf("    0.  LED OFF \n");
    printf("    99. Exit    \n");
    printf("===================\n");
    printf(">>> ");
}

int main(){
    fd = open(GPIO_PATH, O_RDWR);
    if (fd < 0){
        printf("can not open gpio file: %s...\n", GPIO_PATH);
        return -1;
    }

    while(1){
        printMENU();

        scanf("%d", &opt);
        switch(opt){
            case 1:
                printf("TURN LED ON \n");
                ioctl(fd, LED_ON, NULL);
                printf("DONE\n\n\n");
                break;
            case 0:
                printf("TURN LED OFF \n");
                ioctl(fd, LED_OFF, NULL);
                printf("DONE\n\n\n");
                break;
            case 99:
                close(fd);
                exit(1);
                break;
            default:
                printf("Enter invalid opt: %c\n", opt);
                break;
        }
    }
    close(fd);
    return 0;
}