#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

#define PORT 53000

int sockfd;
char buffer[850];
struct sockaddr_in servaddr, cliaddr;
socklen_t c_len, s_len;

void maze_to_buffer();
void change_maze();
void pos_to_buffer(tuple<int, int, int> p );
tuple <int,int,int> buffer_to_pos();
void make_server();
void make_client();

/******************* Function definitions **********************/

void maze_to_buffer(){
    for (int i=0;i<29;i++){
        for (int j=0;j<29;j++){
            buffer[29*i+j] = '0'+ maze[i][j];
        }
    }
}

void change_maze(){
    for (int i=0;i<29;i++){
        for (int j=0;j<29;j++){
            maze[i][j] = buffer[29*i+j] -'0';
        }
    }
}

void pos_to_buffer(tuple<int, int, int> p ){
    string out = to_string(get<0>(p));
    int n = out.size();
    for (int i=n;i<4;i++){
        out = "0"+ out;
    }
    for (int i=0;i<=3;i++){
        buffer[i] = out[i];
    }
    out = to_string(get<1>(p));
    n = out.size();
    for (int i = n;i<4;i++){
        out = '0' + out;
    }
    for (int i=4;i<=7;i++){
        buffer[i] = out[i-4];
    }
    out = to_string(get<2>(p));
    n = out.size();
    for (int i = n;i<4;i++){
        out = '0' + out;
    }
    for (int i=8;i<=11;i++){
        buffer[i] = out[i-8];
    }
}

tuple <int,int,int> buffer_to_pos(){
    int x=0;
    int y=0;
    int angle=0;
    for (int i=3;i>=0;i--){
        x += (buffer[i]-'0')*pow(10,3-i);
    }
    for (int i=7;i>=4;i--){
        y += (buffer[i]-'0')*pow(10,7-i);
    }
    for (int i=11;i>=8;i--){
        angle += (buffer[i]-'0')*pow(10, 11-i);
    }
    return {x,y, angle};
}

void make_server(){
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // Filling server address information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    c_len  = sizeof(cliaddr);   
    
    recvfrom(sockfd , buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
    cout <<"Connected to the client\n";
    maze_to_buffer();
    sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len);
    cout <<"Map information was sent to the client\n";
}

void make_client(){
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    s_len = sizeof(servaddr);
    cout <<"Client is formed\n";

    sendto(sockfd, "Please connect with me", 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );
    cout <<"Request was sent to the server\n";
    recvfrom(sockfd, buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
    cout <<"Map information recieved\n";
    change_maze();    
}