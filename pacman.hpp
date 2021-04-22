#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>




class pacman{

public:

    static SDL_Renderer* Renderer;
    pacman();
    void HandleEvent(SDL_Event* event);
    void update();
    int updateAngle();
    bool tryChanging();
    void keepMoving();


    int frame = 0;
    string nextDirection = "still";
    string currDirection = "still";
    int x;
    int y;
    int nxtRow;
    int nxtCol;
    int row;
    int col;
    int cellWidth = 15;
    int cellHeight = 15;
    int angle=0;
    
};

pacman::pacman(){
    row = nxtRow = col = nxtCol = 1;
    x = cellWidth;
    y = cellHeight; 
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

bool pacman::tryChanging(){
    int tcol;
    int trow;
    if(nextDirection=="still" || nextDirection==currDirection){
        return false;
    }
    else if(nextDirection=="left"){
        if(y%cellWidth!=0){return false;}
        if(x%15==0 && y%15==0){
        	row = y/15;
        	col = x/15;
        }
        trow = row;
        tcol = col-1;
    }
    else if(nextDirection=="right"){
        if(y%cellWidth!=0){return false;}
        if(x%15==0 && y%15==0){
        	row = y/15;
        	col = x/15;
        }
        trow = row;
        tcol = col+1;
        
    }
    else if(nextDirection=="up"){
    	//cout<<"f";
        if(x%cellWidth!=0){return false;}
        if(x%15==0 && y%15==0){
        	row = y/15;
        	col = x/15;
        }
        trow = row-1;
        tcol = col;
        
    }
    else if(nextDirection=="down"){
        if(x%cellWidth!=0){return false;}
        if(x%15==0 && y%15==0){
        	row = y/15;
        	col = x/15;
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
        	if(x%15==0 && y%15==0){
		    	row = y/15;
		    	col = x/15;
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
        	if(x%15==0 && y%15==0){
		    	row = y/15;
		    	col = x/15;
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
        	if(x%15==0 && y%15==0){
		    	row = y/15;
		    	col = x/15;
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
        	if(x%15==0 && y%15==0){
		    	row = y/15;
		    	col = x/15;
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