#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int height = 29;
const int width = 29;

int maze[height][width]={};  //1 for wall and 0 for empty
int visit[height][width]={};


int nr=0,nc=0;

bool rand_unvisited_neighbour(int r,int c){
    vector<pair<int,int>> unvisited;
    if(r+2<height && maze[r+2][c]==0 && visit[r+2][c]==0)unvisited.push_back({r+2,c});
    if(r-2>0 && maze[r-2][c]==0 && visit[r-2][c]==0)unvisited.push_back({r-2,c});
    if(c+2<width/2 && maze[r][c+2]==0 && visit[r][c+2]==0)unvisited.push_back({r,c+2});
    if(c-2>0 && maze[r][c-2]==0 && visit[r][c-2]==0)unvisited.push_back({r,c-2});
    if(unvisited.size()==0){
        return false;
    }
    int random = rand()%(unvisited.size());
    nr = unvisited[random].first;
    nc = unvisited[random].second;
    return true;
}



void initMaze(){
    for(int i=0;i<height;i+=2){
        for(int j=0;j<width;j++){
            maze[i][j]=1;
            visit[i][j]=1;
        }
    }

    for(int i=1;i<height;i+=2){
        for(int j=0;j<width;j+=2){
            maze[i][j]=1;
            visit[i][j]=1;
        }
    }
    maze[2][1]=0;
    maze[1][2]=0;
}



void break_rand_wall(int r,int c){
    vector<pair<int,int>> visited;
    if(r+2<height && maze[r+1][c]==1)visited.push_back({r+2,c});
    if(r-2>0 && maze[r-1][c]==1)visited.push_back({r-2,c});
    if(c+2<=width/2 && maze[r][c+1]==1)visited.push_back({r,c+2});
    if(c-2>0 && maze[r][c-1]==1)visited.push_back({r,c-2});

    int random = rand()%(visited.size());
    nr = visited[random].first;
    nc = visited[random].second;
    maze[(r+nr)/2][(c+nc)/2]=0;
}



void dfsMaze(int r, int c){
    visit[r][c]=1;
    int flag = 0;
    //cout<<r<<" "<<c<<endl;

    while(rand_unvisited_neighbour(r,c)){
        flag = 1;
        maze[(r+nr)/2][(c+nc)/2]=0;
        dfsMaze(nr,nc);
    }

    if(flag==0){
        break_rand_wall(r,c);
    }

}





void mirror(){
    for(int i=0;i<height;i++){
        for(int j=0;j<width/2;j++){
            maze[i][width-j-1]=maze[i][j];
        }
    }
}



void middleWall(){
    for(int i=0;i<6;i++){
        int random = rand()%(height/2-1);
        maze[2*random+1][width/2]=0;
    }
}



void formMaze(){
    srand(time(0));
    initMaze();
    dfsMaze(1,1);
    mirror();
    middleWall();
   /*for(int i=0;i<height;i++){
   	for(int j=0;j<width;j++){
   		cout<<maze[i][j];
   	}
   	cout<<"\n";
   }*/

}
