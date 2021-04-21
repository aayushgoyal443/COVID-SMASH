#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "maze.hpp"
#include "pacman.hpp"


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int cellHeight = 15;
const int cellWidth = 15;

string moveDirection ;

bool init();
bool loadMedia();
void close();

SDL_Texture* loadTexture( std::string path );


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTexture = NULL;

pacman* Pacman = NULL;

bool init()
{
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				//pacman::Renderer = gRenderer;
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

    
	return success;
}


void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


void frender(){
	Pacman->update();
	cout<<Pacman->x<<" "<<Pacman->y<<endl;
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_Rect fillRect = { Pacman->x, Pacman->y, Pacman->cellWidth, Pacman->cellHeight };
    SDL_RenderFillRect(gRenderer, &fillRect );
}

void updateScreen(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			SDL_Rect fillRect = { j*cellWidth, i*cellHeight, cellWidth, cellHeight };
			if(maze[i][j]==1){
				SDL_RenderFillRect( gRenderer, &fillRect );
			}
		}
	}
	frender();
}


void handleEvent(SDL_Event* event){
    Pacman->HandleEvent(event);
}

bool loadMedia(){
return true;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	formMaze();
    Pacman = new pacman();
    const int FPS = 40;
    const int delay = 1000/FPS;
    
    Uint32 frameStart;
    int frameTime;

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				frameStart = SDL_GetTicks();
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					handleEvent(&e);
				}
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );				
				updateScreen();				
                SDL_RenderPresent( gRenderer );	
                frameTime = SDL_GetTicks()-frameStart;
                if(delay>frameTime){
                	SDL_Delay(delay - frameTime);
                }			
			}
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
