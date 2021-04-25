#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "maze.hpp"
#include "pacman.hpp"
#include "initialise.hpp"


using namespace std;



void frender(){
	Pacman->update();

    SDL_Rect fillRect = { Pacman->x, Pacman->y, Pacman->cellWidth, Pacman->cellHeight };
	if (Pacman->updateAngle() == 180){
		SDL_RenderCopyEx(gameRenderer, gPacmanTexture, NULL, &fillRect, 0 , NULL,SDL_FLIP_HORIZONTAL);
	}
	else SDL_RenderCopyEx(gameRenderer, gPacmanTexture, NULL, &fillRect, Pacman->updateAngle() , NULL,SDL_FLIP_NONE );
}

void updateScreen(){
	SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0x00, 0x00, 0xFF );
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			SDL_Rect fillRect = { j*cellWidth, i*cellHeight, cellWidth, cellHeight };
			if(maze[i][j]==1){
				SDL_RenderCopy(gameRenderer, gWallTexture, NULL, &fillRect);
			}
			else{
				SDL_RenderCopy(gameRenderer, gGrassTexture, NULL, &fillRect);
			}
		}
	}
	frender();
}


void handleEvent(SDL_Event* event){
    Pacman->HandleEvent(event);
}


int main(int argc, char *args[])
{
    //Start up SDL and create window
    formMaze();
    Pacman = new pacman();
    const int FPS = 40;
    const int delay = 1000/FPS;
    
    Uint32 frameStart;
    int frameTime;

    if (!init())
    {
        printf("Failed to initialize!\n");
        close();
        return 0;
    }
    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        close();
        return 0;
    }

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Set default current texture
    gameCurrentTexture = gameKeyPressTextures[KEY_MENU];

    //While application is running
    while (!quit)
    {
        //Handle events on queue
        while(!gameRunning && !quit){
            while (SDL_PollEvent(&e) != 0)
            {
                //User requests quit
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                //User presses a key
                else if (e.type == SDL_KEYDOWN)
                {
                    //Select surfaces based on key press
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_m:
                        gameCurrentTexture = gameKeyPressTextures[KEY_MENU];
                        break;

                    case SDLK_F1:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU])
                            //gameCurrentTexture = gameKeyPressTextures[KEY_1P];
                            gameRunning=true;
                        else
                        {
                            // TODO: We can pop-up a message like, Are you sure you want to exit?
                        }
                        break;

                    case SDLK_F2:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU])
                            gameCurrentTexture = gameKeyPressTextures[KEY_2P];
                        else
                        {
                            // TODO: We can pop-up a message like, Are you sure you want to exit?
                        }
                        break;

                    case SDLK_h:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU] || gameCurrentTexture == gameKeyPressTextures[KEY_CREDITS])
                            gameCurrentTexture = gameKeyPressTextures[KEY_HELP];
                        else
                        {
                            // TODO: We can pop-up a message like, Are you sure you want to exit?
                        }
                        break;

                    case SDLK_c:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU] || gameCurrentTexture == gameKeyPressTextures[KEY_HELP])
                            gameCurrentTexture = gameKeyPressTextures[KEY_CREDITS];
                        else
                        {
                            // TODO: We can pop-up a message like, Are you sure you want to exit?
                        }
                        break;

                    default:
                        // gameCurrentTexture = gameKeyPressTextures[KEY_MENU];
                        break;
                    }
                }
                // User presses mouse
                else if ((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) && gameCurrentTexture == gameKeyPressTextures[KEY_MENU])
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    KeyPress_start pos = check_position(x, y);
                    if(e.button.button == SDL_BUTTON_LEFT && pos==1){
                        gameRunning=true;
                    }
                    else if (e.button.button == SDL_BUTTON_LEFT)
                        gameCurrentTexture = gameKeyPressTextures[pos];
                }
            }

            //Clear screen
            SDL_RenderClear(gameRenderer);

            //Render texture to screen
            SDL_RenderCopy(gameRenderer, gameCurrentTexture, NULL, NULL);

            //Update screen
            SDL_RenderPresent(gameRenderer);
        }
        while(gameRunning){
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    gameRunning = false;
                    quit = true;
                }
                handleEvent(&e);
            }
            SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gameRenderer );				
            updateScreen();				
            SDL_RenderPresent( gameRenderer );	
            frameTime = SDL_GetTicks()-frameStart;
            if(delay>frameTime){
                SDL_Delay(delay - frameTime);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}