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
//    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
//    char sendBuff[1025];
//    int numrv;

//    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
//    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
//    serv_addr.sin_addr.s_addr = htonl("224.2.2.2");
    serv_addr.sin_port = htons(4444);
    inet_aton("224.2.2.2", &serv_addr.sin_addr);

//    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
//
//    if(listen(listenfd, 10) == -1)
//    {
//        printf("Failed to listen\n");
//        return -1;
//    }


    while(1)
    {
//        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
    	connfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


        /* Open the file that we wish to transfer */
        FILE *fp = fopen("somefile.som","r");
        if(fp==NULL)
        {
            printf("File open error");
            return 1;
        }

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            unsigned char buff[1316]={0};
            int nread = fread(buff,1,1316,fp);
            printf("Bytes read %d \n", nread);

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
//                write(connfd, buff, nread);
                sendto(connfd, buff, nread, 0, &serv_addr, sizeof(serv_addr));
            }

            /*
             * There is something tricky going on with read ..
             * Either there was error, or we reached end of file.
             */
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
