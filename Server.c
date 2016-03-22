#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
    int connfd = 0;
    struct sockaddr_in dest_addr;
    struct sockaddr_in serv_addr;

    printf("Socket retrieve success\n");

    memset(&dest_addr, '0', sizeof(dest_addr));
    memset(&serv_addr, '0', sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(4444);
    inet_aton("224.2.2.2", &dest_addr.sin_addr);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8001);
    serv_addr.sin_addr.s_addr = inet_addr("10.80.5.3");
    bind(connfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while(1)
    {
    	connfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        bind(connfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    	// File opening part
        FILE *fp = fopen("somefile.som","r");
        if(fp==NULL)
        {
            printf("File open error");
            return 1;
        }

        while(1)
        {
            unsigned char buff[1316]={0};
            int nread = fread(buff,1,1316,fp);
            printf("Bytes read %d \n", nread);

            if(nread > 0)
            {
                printf("Sending \n");
                sendto(connfd, buff, nread, 0, &dest_addr, sizeof(dest_addr));
            }

            // Error handling
            if (nread < 1316)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }

            usleep(150 * 1000);

        }

        close(connfd);
        sleep(1);
    }


    return 0;
}
