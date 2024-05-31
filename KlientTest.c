#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <conio.h>
#include <limits.h>
#include <unistd.h>


#pragma pack(push,1)
typedef struct{
    unsigned char ID;
    unsigned char Shot:1;
    unsigned char KeyboardKey;
    float XPosition;
    float YPosition;
    float XVelocity;
    float YVelocity;
    float Angel;
    float XPositionShot;
    float YPositionShot;
    float XVelocityShot;
    float YVelocityShot;
} Frame;
#pragma pack(pop)

//tutaj jest problem
void sendFrame(SOCKET sockfd, Frame frame)
{
    int result = send(sockfd, (char*)&frame, sizeof(Frame), 0);
    if (result == SOCKET_ERROR) {
        printf("Send failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(1);
    }
}
int main()
{
    WSADATA wsadata;
    SOCKET sockfd;
    struct sockaddr_in serverAddr;
    int result;

    result = WSAStartup(MAKEWORD(2,2),&wsadata);
    if(result!=0)
    {
        printf("WSAStartup failed %d\n",result);
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(sockfd == INVALID_SOCKET)
    {
        printf("Socket create failed %d\n",WSAGetLastError());
        return 1;
    }

    char hostname[100];
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2278);
    gethostname(hostname,sizeof(hostname)-1);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.30");

    result = connect(sockfd, (struct sockaddr*)& serverAddr, sizeof(serverAddr));
    if(result == SOCKET_ERROR)
    {
        printf("Error with connection %d\n",WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else
    {
        printf("Connected with server\n");
    }

    char charToSend='a';

    while(1)
    {

        if(_kbhit())
        {
            char key = _getch();
            if(key=='q'||key=='Q')
            {
                break;
            }
            else if(key=='s'||key=='S')
            {
                printf("przed wyslaniem: %c\n",charToSend);
                Frame frame = {1, 1, charToSend, 10.5f, 20.5f, 1.5f, 2.5f, 45.0f, 15.5f, 25.5f, 3.5f, 4.5f};
                printf("%c\n",frame.KeyboardKey);
                // Send data to server
                sendFrame(sockfd,frame);
                /*result = send(sockfd, (char*)&frame, sizeof(frame), 0);
                if (result == SOCKET_ERROR) {
                    printf("Send failed: %d\n", WSAGetLastError());
                    closesocket(sockfd);
                    WSACleanup();
                    exit(1);
                }*/
            }
            else if(key>='a'&&key<='z')
            {
                charToSend=key;
                printf("%c\n",charToSend);
            }



        }

    }
    closesocket(sockfd);
    WSACleanup();
    return 0;
}