#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "maze.hpp"
#include "pacman.hpp"
#include "bot.hpp"
#include "initialise.hpp"
#include "network.hpp"
using namespace std;


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
                SDL_RenderCopy(gameRenderer, medicineTexture, NULL, &fillRect);
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
    SDL_Rect fillRect = { 0, 700, 250, 100 };
    SDL_RenderCopy(gameRenderer, gQuitTexture, NULL, &fillRect);
    show_pacman();
    Pacman->update();
    Pacman->updateAngle();
    if (texture == bossZombie){
        frender(bossZombie, {Pacman->x, Pacman->y, Pacman-> angle, Pacman->row, Pacman->col,zombie_alive,1 });
    }
    else frender(gPacmanTexture, {Pacman->x, Pacman->y, Pacman-> angle, Pacman->row, Pacman->col,pacmanLives,1 });
}


int main(int argc, char *args[])
{
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    effect1 = Mix_LoadWAV("Resources/pacman_beginning.wav");
    effect2 = Mix_LoadWAV("Resources/pacman_death.wav");
    effect3 = Mix_LoadWAV("Resources/pacman_eatfruit.wav");
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
    const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */        0, 0, "NO" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "YES" },
    };

    const SDL_MessageBoxButtonData leaveButtons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "OK" },      
    };


    const SDL_MessageBoxColorScheme colorScheme = {
        {
            { 0,   0,   0 }, {   0, 255,   0 }, { 255, 255,   0 }, {   0,   0, 255 }, { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        gameWindow, /* .window */
        "WARNING", /* .title */
        "Are you sure you want to quit? All your progress will be lost", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };

    const SDL_MessageBoxData messageboxLeave = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        gameWindow, /* .window */
        "OOPS...", /* .title */
        "The other player has left the game", /* .message */
        SDL_arraysize(leaveButtons), /* .numbuttons */
        leaveButtons, /* .buttons */
        &colorScheme /* .colorScheme */
    };

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
                            // cout<<"hello"<<endl;
                            formMaze();
                            make_server();
                            createNewGame(0);
                            gameServer = true;
                            Mix_PlayChannel(-1, effect1, 0);
                        }
                        break;
                    case SDLK_c:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU] || gameCurrentTexture == gameKeyPressTextures[KEY_HELP])
                                gameCurrentTexture = gameKeyPressTextures[KEY_CREDITS];
                        else if (gameCurrentTexture == gameKeyPressTextures[KEY_2P]){
                            formMaze();
                            make_client();
                            createNewGame(1);
                            gameClient = true;
                            Mix_PlayChannel(-1, effect1, 0);
                        }
                        break;
                    case SDLK_m:
                        gameCurrentTexture = gameKeyPressTextures[KEY_MENU];
                        break;

                    case SDLK_F1:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU]){
                            //gameCurrentTexture = gameKeyPressTextures[KEY_1P];
                            // cout<<"hello"<<endl;
                            gameRunning=true;
                            formMaze();
                            createNewGame(0);
                            Mix_PlayChannel(-1, effect1, 0);
                        }
                        break;

                    case SDLK_F2:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU])
                            gameCurrentTexture = gameKeyPressTextures[KEY_2P];
                        break;

                    case SDLK_r:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU] || gameCurrentTexture == gameKeyPressTextures[KEY_CREDITS])
                            gameCurrentTexture = gameKeyPressTextures[KEY_HELP];
                        break;

                    case SDLK_z:
                        

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
                        formMaze();
                        createNewGame(0);
                        Mix_PlayChannel(-1, effect1, 0);
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
                else if(e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_q){
                    //cout<<"yes"<<endl;
                	int buttonid;
			        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
				        SDL_Log("error displaying message box");
				        return 1;
			        }
			        if (buttonid == -1 || buttonid == 0) {
				        cout<<"no selection"<<endl;
			        } 
                    else {
				        cout<<"selection was "<< buttons[buttonid].text<<endl;
                        gameRunning = false;
                        goBackToMenu();
                        break;
			        }
                }
                handleEvent(&e);
            }

            if(Pacman->powerTime == 300){
                Mix_PlayChannel(-1, effect3, 0);
            }

            SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderClear( gameRenderer );

            // different game winning/losing condiitons
            if (eggsComplete || (!BOT_alive && !BOT2_alive && !BOT3_alive)){
                SDL_RenderCopy(gameRenderer, gPacmanWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
                gameRunning = false;
                goBackToMenu();
                break;
            }
            if (pacmanLives<=0){
                SDL_RenderCopy(gameRenderer, gZombieWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
                gameRunning = false;
                goBackToMenu();
                break;
            }

            // Moving the Pacman			
            updateScreen(gPacmanTexture);				
            eggsComplete = (Pacman->eggsEaten == eggs);
            scared  = Pacman->powerful;
            
            // Handling the bots
            if (BOT_alive){
                BOT->update(Pacman->row, Pacman->col, (Pacman->powerTime)>0);
                BOT->updateAngle();
                int x= Pacman->checkCollision({BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col, BOT_alive, eggsComplete});
                if (x==2){
                    BOT_alive =0;
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
            }
            frender(greenZombie, {BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col, BOT_alive, eggsComplete});
                
            if (BOT2_alive){
                pair< int, int> next2 = BOT2->target(Pacman->row, Pacman->col, Pacman->currDirection, BOT->row, BOT->col);
                BOT2 -> update(next2.first, next2.second, (Pacman->powerTime)>0);
                BOT2->updateAngle();
                int x= Pacman->checkCollision({BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});
                if (x==2){
                    BOT2_alive =0;
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
            }
            frender(blueZombie, {BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});
                
            if (BOT3_alive){
                pair <int, int> next3 = BOT3->changep(Pacman->row, Pacman->col, Pacman->currDirection, 4);
                BOT3->update(next3.first, next3.second, (Pacman->powerTime)>0);
                BOT3->updateAngle();
                int x= Pacman->checkCollision({BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});
                if (x==2){
                    BOT3_alive =0;
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
            }
            frender(pinkZombie, {BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});
            
            SDL_RenderPresent( gameRenderer );	
            frameTime = SDL_GetTicks()-frameStart;
            if(delay>frameTime){
                SDL_Delay(delay - frameTime);
            }
        }

        // When running the game as server
        while (gameServer){
            int leave = 0;
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                
                if( e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                {
                    leave = 1;
                    quit = true;
                }
                else if(e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_q){
                    //cout<<"yes"<<endl;
                	int buttonid;
			        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
				        SDL_Log("error displaying message box");
				        return 1;
			        }
			        if (buttonid == -1 || buttonid == 0) {
				        cout<<"no selection"<<endl;
			        } 
                    else {
				        cout<<"selection was "<< leaveButtons[buttonid].text<<endl;
                        // goBackToMenu();
                        leave = 1;
			        }
                }
                handleEvent(&e);
            }

            if(Pacman->powerTime == 300){
                Mix_PlayChannel(-1, effect3, 0);
            }

            leave_to_buffer(leave);
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len);
            int flag = 0;

            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            flag = buffer_to_leave();

            if(leave == 1){
                gameServer = false;
                goBackToMenu();
                close(sockfd);
                break;
            }
            if(flag == 1){
                int buttonid;
                if (SDL_ShowMessageBox(&messageboxLeave, &buttonid) < 0) {
                    SDL_Log("error displaying message box");
                    return 1;
                }
                gameServer = false;
                
                goBackToMenu();
                close(sockfd);
                break;    
            }

            SDL_SetRenderDrawColor( gameRenderer,  0x00, 0x00, 0x00, 0xFF  );
            SDL_RenderClear( gameRenderer );

            // Different wining/losing conditions
            if (eggsComplete || (!BOT_alive && !BOT2_alive && !BOT3_alive && !zombie_alive)){
                gameServer = false;
                gameClient = false;
            }
            else if (pacmanLives<=0){
                gameServer = false;
                gameClient = false;
            }

            // Moving the pacman
            updateScreen(gPacmanTexture);
            eggsComplete = (Pacman->eggsEaten == eggs);
            scared  = Pacman->powerful? 1:0;
            buffer[0] = '0'+scared;
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len);

            // Sending the maze data
            maze_to_buffer();
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len);

            // getting the position of Enenmy and rendering it
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            tuple<int, int, int, int, int, int, int> enemyPos = buffer_to_pos(); 
            zombie_alive = get<5>(enemyPos);
            frender(bossZombie, enemyPos);
            if (zombie_alive){
                int x =Pacman->checkCollision(enemyPos);
                if (x==2){
                    zombie_alive =0;
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
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
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }                
            }
            pos_to_buffer({BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col, BOT_alive, eggsComplete});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            BOT_alive = buffer[0]-'0';
            frender(greenZombie, {BOT->x, BOT->y, BOT->angle, BOT->row, BOT->col,BOT_alive, eggsComplete});
            
            if (BOT2_alive){
                pair< int, int> next2 = BOT2->target(Pacman->row, Pacman->col, Pacman->currDirection, BOT->row, BOT->col);
                BOT2 -> update(next2.first, next2.second, (Pacman->powerTime)>0);
                BOT2->updateAngle();
                int x= Pacman->checkCollision({BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});
                if (x==2){
                    BOT2_alive =0;
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
            }
            pos_to_buffer({BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            BOT2_alive = buffer[0]-'0';
            frender(blueZombie, {BOT2->x, BOT2->y, BOT2->angle, BOT2->row, BOT2->col, BOT2_alive, eggsComplete});

            if (BOT3_alive){
                pair <int, int> next3 = BOT3->changep(Pacman->row, Pacman->col, Pacman->currDirection, 4);
                BOT3->update(next3.first, next3.second, (Pacman->powerTime)>0);
                BOT3->updateAngle();
                int x= Pacman->checkCollision({BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});
                if (x==2){
                    BOT3_alive =0;
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
                else if (x==1){
                    pacmanLives--;
                    Pacman->row = Pacman->nxtRow = Pacman->col = Pacman->nxtCol = 1;
                    Pacman->x = Pacman->cellWidth;
                    Pacman->y = Pacman->cellHeight; 
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
            }
            pos_to_buffer({BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            BOT3_alive = buffer[0]-'0';
            frender(pinkZombie, {BOT3->x, BOT3->y, BOT3->angle, BOT3->row, BOT3->col, BOT3_alive, eggsComplete});

            // Sending our position to the client
            pos_to_buffer({Pacman->x, Pacman->y, Pacman->angle, Pacman->row, Pacman->col, pacmanLives, eggsComplete});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );

            if (eggsComplete || (!BOT_alive && !BOT2_alive && !BOT3_alive && !zombie_alive)){
                SDL_RenderCopy(gameRenderer, gPacmanWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
            }
            else if (pacmanLives<=0){
                SDL_RenderCopy(gameRenderer, gZombieWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
            }
            else{
                SDL_RenderPresent( gameRenderer );
                frameTime = SDL_GetTicks()-frameStart;
                if(delay>frameTime){
                    SDL_Delay(delay - frameTime);
                }
            }
            if(gameServer == false){
                goBackToMenu();
                close(sockfd);
            }
        }
        
        // When running the game as client
        while (gameClient){
            int leave = 0;
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                {
                    quit = true;
                    leave = 1;
                }
                else if(e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_q){
                    //cout<<"yes"<<endl;
                	int buttonid;
			        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
				        SDL_Log("error displaying message box");
				        return 1;
			        }
			        if (buttonid == -1 || buttonid == 0) {
				        cout<<"no selection"<<endl;
			        } 
                    else {
				        cout<<"selection was "<< buttons[buttonid].text<<endl;
                        // goBackToMenu();
                        leave = 1;
			        }
                }
                if (zombie_alive) handleEvent(&e);  // if not alive we don't need to take care of it
            }
            int flag = 0;
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            flag = buffer_to_leave();

            leave_to_buffer(leave);
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );

            if(leave == 1){
                gameClient = false;
                goBackToMenu();
                close(sockfd);
                break;
            }
            if(flag == 1){
                gameClient = false;
                int buttonid;
                if (SDL_ShowMessageBox(&messageboxLeave, &buttonid) < 0) {
                    SDL_Log("error displaying message box");
                    return 1;
                }       
                goBackToMenu();
                close(sockfd);
                break;    
            }

            SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderClear( gameRenderer );

            // Different winning/losing conditons
            if (eggsComplete || (!BOT_alive && !BOT2_alive && !BOT3_alive && !zombie_alive)){
                gameServer = false;
                gameClient = false;
            }
            else if (pacmanLives<=0){
                gameServer = false;
                gameClient = false;
            }

            // Moving the Monster (if was alive)
            recvfrom(sockfd, buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            scared = buffer[0]-'0';
            updateScreen(bossZombie);

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
            frender(greenZombie, Botpos );

            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            Botpos = buffer_to_pos();
            BOT2_alive = get<5>(Botpos);
            if (!BOT2_alive){
                int x= Pacman->checkCollision(Botpos);
                if (x !=0) BOT2_alive = 1;
            }
            buffer[0] = '0' + BOT2_alive;
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );
            get<5>(Botpos) = BOT2_alive;
            frender(blueZombie, Botpos );

            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            Botpos = buffer_to_pos();
            BOT3_alive = get<5>(Botpos);
            if (!BOT3_alive){
                int x = Pacman->checkCollision(Botpos);
                if (x != 0) BOT3_alive = 1;
            }
            buffer[0] = '0'+BOT3_alive;
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );
            get<5>(Botpos) = BOT3_alive;
            frender(pinkZombie, Botpos );

            // getting the position of server and rendering it
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            tuple<int, int, int, int, int, int, int> PacmanPos = buffer_to_pos();
            pacmanLives = get<5>(PacmanPos);
            eggsComplete = get<6>(PacmanPos);
            frender(gPacmanTexture, PacmanPos);

            if (eggsComplete || (!BOT_alive && !BOT2_alive && !BOT3_alive && !zombie_alive)){
                SDL_RenderCopy(gameRenderer, gPacmanWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
            }
            else if (pacmanLives<=0){
                SDL_RenderCopy(gameRenderer, gZombieWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
            }
            else{
                SDL_RenderPresent( gameRenderer );
                frameTime = SDL_GetTicks()-frameStart;
                if(delay>frameTime){
                    SDL_Delay(delay - frameTime);
                }
            }
            if(gameClient == false){
                goBackToMenu();
                close(sockfd);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}