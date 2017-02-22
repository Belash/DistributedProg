/*
    Simple udp client
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

struct Message
{
    char message[BUFLEN];
    unsigned long long int timestamp;
};
typedef struct Message Message;


int main(void)
{
    struct sockaddr_in si_me, si_other;
    int s, i, slen=sizeof(si_other);
    Message buf;
    Message monMessage;
     int k=0;

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));


    si_me.sin_port = htons(48000);
    //si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    //si_me.sin_addr.s_addr = htonl(48000);
    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    int enabled = 1;
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled));

//bind socket to port
   while (bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1) {
        si_me.sin_port = htons(48000 +k);
//        si_me.sin_addr.s_addr = htonl(48000 +K);
        k=k+100;
    }

    //if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
        //si_me.sin_port = htons(481);

    while(1)
    {
        printf("Enter message : ");
        //gets(&monMessage.message);
        scanf("%s\n",&monMessage.message );
        printf("%s\n",monMessage.message );
        //send the message
        if (sendto(s, &monMessage, sizeof(Message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }
        printf("hi\n" );
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf.message,'\0', BUFLEN);
        //memset(&buf.timestamp,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, &buf, sizeof(monMessage), 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            die("recvfrom()");
        }
        printf("ho\n" );
        printf("%s\n",buf.message);
        //printf("%llu\n",buf.timestamp);
        //puts(buf.message);
        //puts(buf.timestamp);
    }

    close(s);
    return 0;
}
