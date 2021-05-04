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
    string getDirection(int dx, int dy);
    vector<pair<int,int>> getValidCells();
    pair <int, int> changep (int prow, int pcol, string pdir, int offset);
    pair <int, int> target (int prow, int pcol, string pdir, int crow, int ccol);
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


pair <int, int> bot::changep (int prow, int pcol, string pdir, int offset){
    if (pdir == "down") return {prow+offset, pcol };
    else if (pdir =="up") return {prow-offset, pcol};
    else if (pdir =="left") return {prow, pcol-offset};
    else return {prow, pcol+offset};
}

pair <int, int> bot::target (int prow, int pcol, string pdir, int crow, int ccol){
    pair <int, int> newpos = changep(prow, pcol, pdir, 2);
    prow = newpos.first;
    pcol = newpos.second;
    return {2*prow - crow, 2*pcol - ccol};
}



vector<pair<int,int>> bot::getValidCells(){
    vector<pair<int,int>> nextValidCells;
    if(maze[row+1][col]!=1 && currDirection!="up"){
        nextValidCells.push_back({row+1,col});
    }
    if(maze[row-1][col]!=1 && currDirection!="down"){
        nextValidCells.push_back({row-1,col});
    }
    if(maze[row][col+1]!=1 && currDirection!="left"){
        nextValidCells.push_back({row,col+1});
    }
    if(maze[row][col-1]!=1 && currDirection!="right"){
        nextValidCells.push_back({row,col-1});
    }
    return nextValidCells;
}


string bot::getDirection(int dx, int dy){
    if(dx == 1 && dy == 0)return "right";
    if(dx == -1 && dy == 0)return "left";
    if(dx == 0 && dy == 1)return "down";
    if(dx == 0 && dy == -1)return "up";
    return "still";
}

void bot::getNextCell(int prow, int pcol){
    vector<pair<int,int>> nextValidCells = getValidCells();
    int distL = 1e7;
    int distG = 0;
    string turnDirection = "still";
    for(auto j:nextValidCells){
        int tempDist = (j.first-prow)*(j.first-prow) + (j.second-pcol)*(j.second-pcol);
        if(tempDist < distL && mode == 0){
            turnDirection = getDirection( j.second - col, j.first - row);
            distL = tempDist;
        }
        if(tempDist > distG && mode == 1){
            turnDirection = getDirection( j.second - col, j.first - row);
            distG = tempDist;
        }
    }
    currDirection = turnDirection;
}

void bot::update(int prow, int pcol, bool runAway){
    if(runAway)mode = 1;
    else mode = 0;
    if(x%cellWidth == 0 && y%cellWidth == 0){
        row = y/cellWidth;
        col = x/cellWidth;
        getNextCell(prow, pcol);
        if(currDirection == "left"){
            nxtRow = row;
            nxtCol = col-1;
        }
        else if(currDirection == "right"){
            nxtRow = row;
            nxtCol = col+1;
        }
        else if(currDirection == "down"){
            nxtRow = row+1;
            nxtCol = col;
        }
        else if(currDirection == "up"){
            nxtRow = row-1;
            nxtCol = col;
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
