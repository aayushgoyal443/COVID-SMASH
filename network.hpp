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
void pos_to_buffer(tuple<int, int, int, int, int, int, int> p );
tuple <int,int,int,int,int, int, int> buffer_to_pos();
void make_server();
void make_client();

/******************* Function definitions **********************/

void leave_to_buffer(int leave){
    for (int i=0;i<22;i++){
        buffer[i] = '0'+ leave;
    }
}

int buffer_to_leave(){
    if(buffer[0]=='1')return 1;
    else return 0;
}

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

// {x, y, angle, row, col, lives/alive, eggs done?}
void pos_to_buffer(tuple<int, int, int, int, int, int, int> p ){
    string out = to_string(get<0>(p));  // x coordinate
    int n = out.size();
    for (int i=n;i<4;i++){
        out = "0"+ out;
    }
    for (int i=0;i<=3;i++){
        buffer[i] = out[i];
    }
    out = to_string(get<1>(p)); // y coordinate
    n = out.size();
    for (int i = n;i<4;i++){
        out = '0' + out;
    }
    for (int i=4;i<=7;i++){
        buffer[i] = out[i-4];
    }
    out = to_string(get<2>(p)); // angle
    n = out.size();
    for (int i = n;i<4;i++){
        out = '0' + out;
    }
    for (int i=8;i<=11;i++){
        buffer[i] = out[i-8];
    }
    out = to_string(get<3>(p)); // row number
    n = out.size();
    for (int i = n;i<4;i++){
        out = '0' + out;
    }
    for (int i=12;i<=15;i++){
        buffer[i] = out[i-12];
    }
    out = to_string(get<4>(p)); // col number
    n = out.size();
    for (int i = n;i<4;i++){
        out = '0' + out;
    }
    for (int i=16;i<=19;i++){
        buffer[i] = out[i-16];
    }
    buffer[20] = get<5>(p)+'0'; // 0 if dead, 1 if alive
    buffer[21] = get<6>(p)+'0'; // 1 if all the eggs are eaten
}

// {x, y, angle, row, col, lives/alive, eggs done?}
tuple <int,int,int,int,int, int, int> buffer_to_pos(){
    int x=0;
    int y=0;
    int angle=0;
    int row =0;
    int col=0;
    for (int i=3;i>=0;i--){
        x += (buffer[i]-'0')*pow(10,3-i);
    }
    for (int i=7;i>=4;i--){
        y += (buffer[i]-'0')*pow(10,7-i);
    }
    for (int i=11;i>=8;i--){
        angle += (buffer[i]-'0')*pow(10, 11-i);
    }
    for (int i=15;i>=12;i--){
        row += (buffer[i]-'0')*pow(10, 15-i);
    }
    for (int i=19;i>=16;i--){
        col += (buffer[i]-'0')*pow(10, 19-i);
    }
    int alive = buffer[20]-'0';
    int eggs = buffer[21]-'0';
    return {x,y, angle, row, col, alive, eggs};
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