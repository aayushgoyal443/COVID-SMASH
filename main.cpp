#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "maze.hpp"
#include "pacman.hpp"
#include "bot.hpp"
#include "initialise.hpp"
#include "network.hpp"


using namespace std;

void frender(SDL_Texture* texture, tuple<int,int,int,int,int> pos){
    SDL_Rect fillRect = { get<0>(pos), get<1>(pos), Pacman->cellWidth, Pacman->cellHeight };
	if (get<2>(pos) == 180){
		SDL_RenderCopyEx(gameRenderer, texture, NULL, &fillRect, 0 , NULL,SDL_FLIP_HORIZONTAL);
	}
	else SDL_RenderCopyEx(gameRenderer, texture, NULL, &fillRect, get<2>(pos) , NULL,SDL_FLIP_NONE );
}

void updateScreen(SDL_Texture* texture){
	SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0x00, 0x00, 0xFF );
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			SDL_Rect fillRect = { j*cellWidth, i*cellHeight, cellWidth, cellHeight };
			if(maze[i][j]==1){
				SDL_RenderCopy(gameRenderer, gWallTexture, NULL, &fillRect);
			}
			else if(maze[i][j]==2){
				SDL_RenderCopy(gameRenderer, gGrassTexture, NULL, &fillRect);
                SDL_RenderCopy(gameRenderer, eggTexture, NULL, &fillRect);
			}
            else if(maze[i][j]==3){
				SDL_RenderCopy(gameRenderer, gGrassTexture, NULL, &fillRect);
                SDL_RenderCopy(gameRenderer, vaccineTexture, NULL, &fillRect);
			}
            else{
                SDL_RenderCopy(gameRenderer, gGrassTexture, NULL, &fillRect);
            }
		}
	}
    Pacman->update();
    Pacman->updateAngle();
    frender(texture, {Pacman->x, Pacman->y, Pacman-> angle, Pacman->row, Pacman->col});
}

void handleEvent(SDL_Event* event){
    Pacman->HandleEvent(event);
}


int main(int argc, char *args[])
{
    //Start up SDL and create window
    formMaze();
    Pacman = new pacman();
    BOT = new bot();
    BOT2 = new bot();
    BOT3 = new bot();
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
        while(!gameRunning && !quit && !gameServer && !gameClient){
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
                    case SDLK_s:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_2P]){
                            make_server();
                            Pacman->type = 0;
                            gameServer = true;
                        }
                        break;
                    case SDLK_c:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_2P]){
                            make_client();
                            Pacman->type =1;
                            gameClient = true;
                        }
                        break;
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

                    case SDLK_z:
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

        // While running in Single player
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
            updateScreen(gPacmanTexture);				
            
            BOT->update(Pacman->row, Pacman->col, (Pacman->powerTime)>0);
            BOT->updateAngle();
            pair< int, int> next2 = BOT2->target(Pacman->row, Pacman->col, Pacman->currDirection, BOT->row, BOT->col);
            BOT2 -> update(next2.first, next2.second, (Pacman->powerTime)>0);
            BOT2->updateAngle();
            pair <int, int> next3 = BOT3->changep(Pacman->row, Pacman->col, Pacman->currDirection, 4);
            BOT3->update(next3.first, next3.second, (Pacman->powerTime)>0);
            BOT3->updateAngle();	
            frender(gZombieTexture, {BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col});
            frender(gZombieTexture, {BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col});
            frender(gZombieTexture, {BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col});
            Pacman->checkCollision({BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col});
            Pacman->checkCollision({BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col});
            Pacman->checkCollision({BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col});

            SDL_RenderPresent( gameRenderer );	
            frameTime = SDL_GetTicks()-frameStart;
            if(delay>frameTime){
                SDL_Delay(delay - frameTime);
            }
        }

        // When running in Double player
        while (gameServer || gameClient){
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    gameServer = false;
                    gameClient = false;
                    quit = true;
                    close(sockfd);
                }
                handleEvent(&e);
            }
            SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gameRenderer );

            if (gameServer){
                updateScreen(gPacmanTexture);
                // getting the position of Enenmy and rendering it
                recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
                // cout <<"Position of the client recieved\n";
                tuple<int, int, int, int, int> enemyPos = buffer_to_pos(); 
                frender(gZombieTexture, enemyPos);
                Pacman->checkCollision(enemyPos);

                // Sending our position to the client
                pos_to_buffer({Pacman->x, Pacman->y, Pacman->angle, Pacman->row, Pacman->col});
                sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
                maze_to_buffer();
                sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len);
                // cout <<"Position was sent to the client\n";

                BOT->update(Pacman->row, Pacman->col, (Pacman->powerTime)>0);
                BOT->updateAngle();
                frender(gZombieTexture, {BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col});
                pos_to_buffer({BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col});
                sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
                Pacman->checkCollision({BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col});

                pair< int, int> next2 = BOT2->target(Pacman->row, Pacman->col, Pacman->currDirection, get<3>(enemyPos), get<4>(enemyPos)); //this way it will assist the enemy
                BOT2 -> update(next2.first, next2.second, (Pacman->powerTime)>0);
                BOT2->updateAngle();
                frender(gZombieTexture, {BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col});
                pos_to_buffer({BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col});
                sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
                Pacman->checkCollision({BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col});

                pair <int, int> next3 = BOT3->changep(Pacman->row, Pacman->col, Pacman->currDirection, 4);
                BOT3->update(next3.first, next3.second, (Pacman->powerTime)>0);
                BOT3->updateAngle();
                frender(gZombieTexture, {BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col});
                pos_to_buffer({BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col});
                sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
                Pacman->checkCollision({BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col});

            }
            else if (gameClient){
                updateScreen(gZombieTexture);
                // Sending our position to the server
                pos_to_buffer({Pacman->x, Pacman->y, Pacman->angle, Pacman->row, Pacman->col});
                sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );
                // cout <<"Client position was sent to the server\n";

                // getting the position of server and rendering it
                recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
                // cout <<"Server position received\n";
                frender(gPacmanTexture, buffer_to_pos());
                recvfrom(sockfd, buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
                //cout <<"Map information recieved\n";
                change_maze(); 

                recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
                frender(gZombieTexture, buffer_to_pos());

                recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
                frender(gZombieTexture, buffer_to_pos());

                recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
                frender(gZombieTexture, buffer_to_pos());
                
            }

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