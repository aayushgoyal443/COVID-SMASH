#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>

#define POWER_TIME 300

class pacman{
public:
    static SDL_Renderer* Renderer;
    pacman(int n);
    void HandleEvent(SDL_Event* event);
    int checkCollision(tuple<int,int,int,int,int,int,int> posMonster);
    void update();
    int updateAngle();
    bool tryChanging();
    void keepMoving();
    int type = 0; // 0 for pacman and 1 for monster
    int eggsEaten = 0;
    int frame = 0;
    bool powerful = false;
    int powerTime = 0;
    string nextDirection = "still";
    string currDirection = "still";
    int x;
    int y;
    int nxtRow;
    int nxtCol;
    int row;
    int col;
    int cellWidth = 24;
    int cellHeight = 24;
    int angle=0;    
};

pacman::pacman(int n){
    type = n;
    if (type ==0){
        row = nxtRow = col = nxtCol = 1;
        x = cellWidth;
        y = cellHeight; 
    }
    else{   // Monster has to be kept at the opposite side of Maze
        row = col = 27;
        x = y = row*cellWidth;
    }
}

void pacman::HandleEvent(SDL_Event* event){
    if( event->type == SDL_KEYDOWN )
    {
        switch( event->key.keysym.sym )
        {
            case SDLK_UP: {nextDirection = "up"; break;}
            case SDLK_DOWN: nextDirection = "down"; break;
            case SDLK_LEFT: nextDirection = "left"; break;
            case SDLK_RIGHT: nextDirection = "right"; break;
        }
    }
    
}

int pacman::checkCollision(tuple<int,int,int,int,int,int,int> posMonster){
    int rowMonster = get<3>(posMonster);
    int colMonster = get<4>(posMonster);
    int xMonster = get<0>(posMonster);
    int yMonster = get<1>(posMonster); 
    if(abs(xMonster- x)<=5 && abs(yMonster- y)<=5){
        if(powerful){
            cout<<"Monster Killed\n";
            return 2;
        }
        else{
            cout<<"Pacman Killed\n";
            return 1;
        }        
    }
    return 0;   // when nothing happens
}

bool pacman::tryChanging(){
    int tcol;
    int trow;
    if(nextDirection=="still" || nextDirection==currDirection){
        return false;
    }
    else if(nextDirection=="left"){
        if(y%cellWidth!=0){return false;}
        if(x%cellWidth==0 && y%cellWidth==0){
        	row = y/cellWidth;
        	col = x/cellWidth;
            if(maze[row][col]==2 && type ==0){
                maze[row][col]=0;
                eggsEaten++;
            }
            else if(maze[row][col]==3 && type ==0){
                maze[row][col]=0;
                powerTime = POWER_TIME;
            }
        }
        trow = row;
        tcol = col-1;
    }
    else if(nextDirection=="right"){
        if(y%cellWidth!=0){return false;}
        if(x%cellWidth==0 && y%cellWidth==0){
        	row = y/cellWidth;
        	col = x/cellWidth;
            if(maze[row][col]==2 && type ==0){
                maze[row][col]=0;
                eggsEaten++;
            }
            else if(maze[row][col]==3 && type ==0){
                maze[row][col]=0;
                powerTime = POWER_TIME;
            }
        }
        trow = row;
        tcol = col+1;
        
    }
    else if(nextDirection=="up"){
    	//cout<<"f";
        if(x%cellWidth!=0){return false;}
        if(x%cellWidth==0 && y%cellWidth==0){
        	row = y/cellWidth;
        	col = x/cellWidth;
            if(maze[row][col]==2 && type==0){
                maze[row][col]=0;
                eggsEaten++;
            }
            else if(maze[row][col]==3 && type ==0){
                maze[row][col]=0;
                powerTime = POWER_TIME;
            }
        }
        trow = row-1;
        tcol = col;
        
    }
    else if(nextDirection=="down"){
        if(x%cellWidth!=0){return false;}
        if(x%cellWidth==0 && y%cellWidth==0){
        	row = y/cellWidth;
        	col = x/cellWidth;
            if(maze[row][col]==2 && type==0){
                maze[row][col]=0;
                eggsEaten++;
            }
            else if(maze[row][col]==3 && type ==0){
                maze[row][col]=0;
                powerTime = POWER_TIME;
            }
        }
        trow = row+1;
        tcol = col;
        
    }

    if(maze[trow][tcol]==1)return false;
    nxtRow = trow;
    nxtCol = tcol;
    currDirection = nextDirection;
    nextDirection = "still";
    return true;
}

void pacman::keepMoving(){
    if(currDirection == "still")return;
    else if(currDirection=="left"){
        if(x%cellWidth!=0){x=x-3;return;}
        else{
        	if(x%cellWidth==0 && y%cellWidth==0){
		    	row = y/cellWidth;
		    	col = x/cellWidth;
                if(maze[row][col]==2 && type==0){
                    maze[row][col]=0;
                    eggsEaten++;
                }
                else if(maze[row][col]==3 && type ==0){
                    maze[row][col]=0;
                    powerTime = POWER_TIME;
                }
		    }
            if(maze[row][col-1]==1){
                currDirection = "still";
            }
            else{
            	nxtCol = col-1;
            	nxtRow = row;
                x-=3;
            }
        }
    }
    else if(currDirection=="right"){
        if(x%cellWidth!=0){x+=3;return;}
        else{
        	if(x%cellWidth==0 && y%cellWidth==0){
		    	row = y/cellWidth;
		    	col = x/cellWidth;
                if(maze[row][col]==2 && type==0){
                    maze[row][col]=0;
                    eggsEaten++;
                }
                else if(maze[row][col]==3 && type ==0){
                    maze[row][col]=0;
                    powerTime = POWER_TIME;
                }
		    }
            if(maze[row][col+1]==1){
                currDirection = "still";
            }
            else{
                nxtCol = col+1;
            	nxtRow = row;
                x+=3;
            }
        }
    }
    else if(currDirection=="up"){
        if(y%cellWidth!=0){y-=3;return;}
        else{
        	if(x%cellWidth==0 && y%cellWidth==0){
		    	row = y/cellWidth;
		    	col = x/cellWidth;
                if(maze[row][col]==2 && type==0){
                    maze[row][col]=0;
                    eggsEaten++;
                }
                else if(maze[row][col]==3 && type ==0){
                    maze[row][col]=0;
                    powerTime = POWER_TIME;
                }
		    }
            if(maze[row-1][col]==1){
                currDirection = "still";
            }
            else{
                nxtCol = col;
            	nxtRow = row-1;
                y-=3;
            }
        }
    }
    else if(currDirection=="down"){
        if(y%cellWidth!=0){y+=3;return;}
        else{
        	if(x%cellWidth==0 && y%cellWidth==0){
		    	row = y/cellWidth;
		    	col = x/cellWidth;
                if(maze[row][col]==2 && type==0){
                    maze[row][col]=0;
                    eggsEaten++;
                }
                else if(maze[row][col]==3 && type ==0){
                    maze[row][col]=0;
                    powerTime = POWER_TIME;
                }
		    }
            if(maze[row+1][col]==1){
                currDirection = "still";
            }
            else{
                nxtCol = col;
            	nxtRow = row+1;
                y+=3;
            }
        }
    }
}

void pacman::update(){
    if(type == 0){
        powerTime = max(0,powerTime-1);
        if(powerTime>0)powerful=true;
        else powerful = false;
    }
    if(!tryChanging())keepMoving();
}

int pacman::updateAngle(){
    if (currDirection =="still") return angle;  // this will retain the angle
    if (currDirection =="down") angle = 90;
    else if (currDirection == "left") angle = 180;
    else if (currDirection =="up") angle = 270;
    else if (currDirection == "right") angle = 0;
    return angle;   // else we will update the angle
}