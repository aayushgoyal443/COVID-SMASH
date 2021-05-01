#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>

class bot{
public:
    bot();
    void update(int prow, int pcol, bool runAway);
    void getNextCell(int prow, int pcol);
    int favourUp(int prow, int pcol);
    int favourDown(int prow, int pcol);
    int favourLeft(int prow, int pcol);
    int favourRight(int prow, int pcol);
    int updateAngle();
    string currDirection = "still";
    string nextDirection = "still";
    int mode = 0; // 0-chase and 1-run away
    int x;
    int y;
    int nxtRow;
    int nxtCol;
    int row;
    int col;
    int cellWidth = 24;
    int cellHeight = 24;
    int angle = 0;
};






bot::bot(){
    row = col = 27;
    x = y = row*cellWidth;
}

int bot::favourLeft(int prow, int pcol){
    int trow = row;
    int tcol = col-1;
    if(maze[trow][tcol]==1)return -1;
    int disPrev = (row-prow)*(row-prow)+(col-pcol)*(col-pcol);
    int distNew = (trow-prow)*(trow-prow)+(tcol-pcol)*(tcol-pcol);
    if(mode==0){
        if(distNew<disPrev) return 2;
        if(distNew == disPrev)return 1;
    }
    if(mode==1){
        if(distNew > disPrev) return 2;
        if(distNew == disPrev)return 1;
    }
    return 0;
}

int bot::favourRight(int prow, int pcol){
    int trow = row;
    int tcol = col+1;
    if(maze[trow][tcol]==1)return -1;
    int disPrev = (row-prow)*(row-prow)+(col-pcol)*(col-pcol);
    int distNew = (trow-prow)*(trow-prow)+(tcol-pcol)*(tcol-pcol);
    if(mode==0){
        if(distNew<disPrev) return 2;
        if(distNew == disPrev)return 1;
    }
    if(mode==1){
        if(distNew > disPrev) return 2;
        if(distNew == disPrev)return 1;
    }
    return 0;
}

int bot::favourUp(int prow, int pcol){
    int trow = row -1;
    int tcol = col;
    if(maze[trow][tcol]==1)return -1;
    int disPrev = (row-prow)*(row-prow)+(col-pcol)*(col-pcol);
    int distNew = (trow-prow)*(trow-prow)+(tcol-pcol)*(tcol-pcol);
    if(mode==0){
        if(distNew<disPrev) return 2;
        if(distNew == disPrev)return 1;
    }
    if(mode==1){
        if(distNew > disPrev) return 2;
        if(distNew == disPrev)return 1;
    }
    return 0;
}

int bot::favourDown(int prow, int pcol){
    int trow = row + 1;
    int tcol = col;
    if(maze[trow][tcol]==1)return -1;
    int disPrev = (row-prow)*(row-prow)+(col-pcol)*(col-pcol);
    int distNew = (trow-prow)*(trow-prow)+(tcol-pcol)*(tcol-pcol);
    if(mode==0){
        if(distNew<disPrev) return 2;
        if(distNew == disPrev)return 1;
    }
    if(mode==1){
        if(distNew > disPrev) return 2;
        if(distNew == disPrev)return 1;
    }
    return 0;
}

void bot::getNextCell(int prow, int pcol){
    int t1 = favourLeft(prow,pcol);
    int t2 = favourUp(prow,pcol);
    int t3 = favourRight(prow,pcol);
    int t4 = favourDown(prow,pcol);
    if(t1 == 2 && currDirection!="right"){
        currDirection = "left";
        return;
    }
    if(t2 == 2 && currDirection!="down"){
        currDirection = "up";
        return;
    }
    if(t3 == 2 && currDirection!="left"){
        currDirection = "right";
        return;
    }
    if(t4 == 2 && currDirection!="up"){
        currDirection = "down";
        return;
    }
    if(t1 == 1 && currDirection!="right"){
        currDirection = "left";
        return;
    }
    if(t2 == 1 && currDirection!="down"){
        currDirection = "up";
        return;
    }
    if(t3 == 1 && currDirection!="left"){
        currDirection = "right";
        return;
    }
    if(t4 == 1 && currDirection!="up"){
        currDirection = "down";
        return;
    }
    if(t1 == 0 && currDirection!="right"){
        currDirection = "left";
        return;
    }
    if(t2 == 0 && currDirection!="down"){
        currDirection = "up";
        return;
    }
    if(t3 == 0 && currDirection!="left"){
        currDirection = "right";
        return;
    }
    if(t4 == 0 && currDirection!="up"){
        currDirection = "down";
        return;
    }
}

void bot::update(int prow, int pcol, bool runAway){
    if(runAway)mode = 1;
    else mode = 0;
    if(x%cellWidth == 0 && y%cellWidth == 0){
        getNextCell(prow, pcol);
        if(currDirection == "left"){
            col--;
        }
        else if(currDirection == "right"){
            col++;
        }
        else if(currDirection == "down"){
            row++;
        }
        else if(currDirection == "up"){
            row--;
        }
    }
    if(currDirection == "still")return;
    else if(currDirection == "left"){
        x-=3;
    }
    else if(currDirection == "right"){
        x+=3;
    }
    else if(currDirection == "down"){
        y+=3;
    }
    else if(currDirection == "up"){
        y-=3;
    }
}


int bot::updateAngle(){
    if (currDirection =="still") return angle;  // this will retain the angle
    if (currDirection =="down") angle = 90;
    else if (currDirection == "left") angle = 180;
    else if (currDirection =="up") angle = 270;
    else if (currDirection == "right") angle = 0;
    return angle;   // else we will update the angle
}
