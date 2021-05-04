#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "maze.hpp"
#include "pacman.hpp"
#include "bot.hpp"
#include "initialise.hpp"
#include "network.hpp"


using namespace std;

void frender(SDL_Texture* texture, tuple<int,int,int,int,int, int, int> pos){
    SDL_Rect fillRect = { get<0>(pos), get<1>(pos), Pacman->cellWidth, Pacman->cellHeight };
    if (texture == gZombieTexture && get<5>(pos)==0) texture= deadZombieTexture;
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
    if (texture == gZombieTexture){
        frender(gZombieTexture, {Pacman->x, Pacman->y, Pacman-> angle, Pacman->row, Pacman->col,zombie_alive,1 });
    }
    else frender(gPacmanTexture, {Pacman->x, Pacman->y, Pacman-> angle, Pacman->row, Pacman->col,pacmanLives,1 });
}

void handleEvent(SDL_Event* event){
    Pacman->HandleEvent(event);
}


int main(int argc, char *args[])
{
    //Start up SDL and create window
    formMaze();
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
        //Going to the Main Menu
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
                            Pacman = new pacman(0);
                            gameServer = true;
                        }
                        break;
                    case SDLK_c:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_2P]){
                            make_client();
                            Pacman = new pacman(1);
                            gameClient = true;
                        }
                        break;
                    case SDLK_m:
                        gameCurrentTexture = gameKeyPressTextures[KEY_MENU];
                        break;

                    case SDLK_F1:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU]){
                            //gameCurrentTexture = gameKeyPressTextures[KEY_1P];
                            gameRunning=true;
                            Pacman = new pacman(0);
                        }
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
                        Pacman = new pacman(0);
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
                if( e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                {
                    gameRunning = false;
                    quit = true;
                }
                handleEvent(&e);
            }

            // different game winning/losing condiitons
            if (eggsComplete){
                cout <<"Pacman won the game\n";
                gameRunning = false;
                quit = true;
            }
            if (pacmanLives<=0){
                cout <<"Zombies won the game\n";
                gameRunning = false;
                quit = true;
            }
            if (!BOT_alive && !BOT2_alive && !BOT3_alive){
                cout <<"Pacman Won the game\n";
                gameRunning = false;
                quit = true;
            }

            SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gameRenderer );	

            // Moving the Pacman			
            updateScreen(gPacmanTexture);				
            eggsComplete = (Pacman->eggsEaten == eggs);
            
            // Handling the bots
            if (BOT_alive){
                BOT->update(Pacman->row, Pacman->col, (Pacman->powerTime)>0);
                BOT->updateAngle();
                int x= Pacman->checkCollision({BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col, BOT_alive, eggsComplete});
                if (x==2){
                    BOT_alive =0;
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                }
            }
            frender(gZombieTexture, {BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col, BOT_alive, eggsComplete});
                
            if (BOT2_alive){
                pair< int, int> next2 = BOT2->target(Pacman->row, Pacman->col, Pacman->currDirection, BOT->row, BOT->col);
                BOT2 -> update(next2.first, next2.second, (Pacman->powerTime)>0);
                BOT2->updateAngle();
                int x= Pacman->checkCollision({BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});
                if (x==2){
                    BOT2_alive =0;
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                }
            }
            frender(gZombieTexture, {BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});
                
            if (BOT3_alive){
                pair <int, int> next3 = BOT3->changep(Pacman->row, Pacman->col, Pacman->currDirection, 4);
                BOT3->update(next3.first, next3.second, (Pacman->powerTime)>0);
                BOT3->updateAngle();
                int x= Pacman->checkCollision({BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});
                if (x==2){
                    BOT3_alive =0;
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                }
            }
            frender(gZombieTexture, {BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});

            SDL_RenderPresent( gameRenderer );	
            frameTime = SDL_GetTicks()-frameStart;
            if(delay>frameTime){
                SDL_Delay(delay - frameTime);
            }
        }

        // When running the game as server
        while (gameServer){
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                {
                    gameServer = false;
                    gameClient = false;
                    quit = true;
                    close(sockfd);
                }
                handleEvent(&e);
            }

            // Different wining/losing conditions
            if (eggsComplete){
                // information regarding this needs to be sent
                cout <<"Pacman won the game\n";
                gameServer = false;
                gameClient = false;
                quit = true;
            }
            if (pacmanLives<=0){
                cout <<"Zombies won the game\n";
                gameServer = false;
                gameClient = false;
                quit = true;
            }
            if (!BOT_alive && !BOT2_alive && !BOT3_alive && !zombie_alive){
                cout <<"Pacman won the game\n";
                gameServer = false;
                gameClient = false;
                quit = true;
            }
            SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gameRenderer );

            // Moving the pacman
            updateScreen(gPacmanTexture);
            eggsComplete = (Pacman->eggsEaten == eggs);

            // Sending the maze data
            maze_to_buffer();
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len);

            // getting the position of Enenmy and rendering it
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            tuple<int, int, int, int, int, int, int> enemyPos = buffer_to_pos(); 
            zombie_alive = get<5>(enemyPos);
            frender(gZombieTexture, enemyPos);
            if (zombie_alive){
                int x =Pacman->checkCollision(enemyPos);
                if (x==2){
                    zombie_alive =0;
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                }
            }
            buffer[0] = '0'+ zombie_alive;
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );

            // Handling the bots
            if (BOT_alive){
                BOT->update(Pacman->row, Pacman->col, (Pacman->powerTime)>0);
                BOT->updateAngle();
                int x= Pacman->checkCollision({BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col, BOT_alive, eggsComplete});
                if (x==2){
                    BOT_alive =0;
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                }                
            }
            pos_to_buffer({BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col, BOT_alive, eggsComplete});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            BOT_alive = buffer[0]-'0';
            frender(gZombieTexture, {BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col,BOT_alive, eggsComplete});
            
            if (BOT2_alive){
                pair< int, int> next2 = BOT2->target(Pacman->row, Pacman->col, Pacman->currDirection, BOT->row, BOT->col);
                BOT2 -> update(next2.first, next2.second, (Pacman->powerTime)>0);
                BOT2->updateAngle();
                int x= Pacman->checkCollision({BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});
                if (x==2){
                    BOT2_alive =0;
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                }
            }
            pos_to_buffer({BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            BOT2_alive = buffer[0]-'0';
            frender(gZombieTexture, {BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});

            if (BOT3_alive){
                pair <int, int> next3 = BOT3->changep(Pacman->row, Pacman->col, Pacman->currDirection, 4);
                BOT3->update(next3.first, next3.second, (Pacman->powerTime)>0);
                BOT3->updateAngle();
                int x= Pacman->checkCollision({BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});
                if (x==2){
                    BOT3_alive =0;
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                }
            }
            pos_to_buffer({BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            BOT3_alive = buffer[0]-'0';
            frender(gZombieTexture, {BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});

            // Sending our position to the client
            pos_to_buffer({Pacman->x, Pacman->y, Pacman->angle, Pacman->row, Pacman->col, pacmanLives, eggsComplete});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );

            SDL_RenderPresent( gameRenderer );
            frameTime = SDL_GetTicks()-frameStart;
            if(delay>frameTime){
                SDL_Delay(delay - frameTime);
            }
        }
        
        // When running the game as client
        while (gameClient){
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                {
                    gameServer = false;
                    gameClient = false;
                    quit = true;
                    close(sockfd);
                }
                if (zombie_alive) handleEvent(&e);  // if not alive we don't need to take care of it
            }

            // Different winning/losing conditons
            if (eggsComplete){
                // information regarding this needs to be sent
                cout <<"Pacman won the game\n";
                gameServer = false;
                gameClient = false;
                quit = true;
            }
            if (pacmanLives<=0){
                cout <<"Zombies won the game\n";
                gameServer = false;
                gameClient = false;
                quit = true;
            }
            if (!BOT_alive && !BOT2_alive && !BOT3_alive && !zombie_alive){
                cout <<"Pacman won the game\n";
                gameServer = false;
                gameClient = false;
                quit = true;
            }
            SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gameRenderer );

            // Moving the Monster (if was alive)
            updateScreen(gZombieTexture);

            // Changing the eaten eggs and vaccine info
            recvfrom(sockfd, buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            change_maze();

            // Sending our position to the server
            pos_to_buffer({Pacman->x, Pacman->y, Pacman->angle, Pacman->row, Pacman->col, zombie_alive, 0});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );

            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            zombie_alive = buffer[0]-'0';   // TO know if the Pacman killed us, effect will be visible in the next iteration or both the Server and client

            // Handling the bots
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            tuple<int, int, int, int, int, int, int> Botpos = buffer_to_pos();
            BOT_alive = get<5>(Botpos);
            if (!BOT_alive){
                int x= Pacman->checkCollision(Botpos);
                if (x !=0) BOT_alive = 1;
            }
            buffer[0] = '0'+BOT_alive;
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );
            get<5>(Botpos) = BOT_alive;
            frender(gZombieTexture, Botpos );

            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            Botpos = buffer_to_pos();
            BOT2_alive = get<5>(Botpos);
            if (!BOT2_alive){
                int x= Pacman->checkCollision(Botpos);
                if (x !=0) BOT2_alive = 1;
            }
            buffer[0] = '0'+BOT2_alive;
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );
            get<5>(Botpos) = BOT2_alive;
            frender(gZombieTexture, Botpos );

            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            Botpos = buffer_to_pos();
            BOT3_alive = get<5>(Botpos);
            if (!BOT3_alive){
                int x= Pacman->checkCollision(Botpos);
                if (x !=0) BOT3_alive = 1;
            }
            buffer[0] = '0'+BOT3_alive;
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );
            get<5>(Botpos) = BOT3_alive;
            frender(gZombieTexture, Botpos );

            // getting the position of server and rendering it
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            tuple<int, int, int, int, int, int, int> PacmanPos = buffer_to_pos();
            pacmanLives = get<5>(PacmanPos);
            eggsComplete = get<6>(PacmanPos);
            frender(gPacmanTexture, PacmanPos);

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