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
    struct sockaddr_in serv_addr;

    printf("Socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4444);
    inet_aton("224.2.2.2", &serv_addr.sin_addr);

    while(1)
    {
    	connfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

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
                sendto(connfd, buff, nread, 0, &serv_addr, sizeof(serv_addr));
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

            usleep(1 * 1000);

        }

        close(connfd);
        sleep(1);
    }


    return 0;
}
