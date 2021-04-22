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

SDL_Texture* gWallTexture = NULL;
SDL_Texture* gGrassTexture = NULL;
SDL_Texture* gPacmanTexture = NULL;

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

bool loadMedia(){
	bool success = true;

    //Load the walls
    gPacmanTexture = loadTexture("Resources/pacman.png");
    if (gPacmanTexture == NULL)
    {
        printf("Failed to load Pacman image!\n");
        success = false;
    }

	gWallTexture = loadTexture("Resources/wall.png");
    if (gWallTexture == NULL)
    {
        printf("Failed to load Pacman image!\n");
        success = false;
    }

	gGrassTexture = loadTexture("Resources/grass.png");
    if (gGrassTexture == NULL)
    {
        printf("Failed to load Pacman image!\n");
        success = false;
    }

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gWallTexture );
	gWallTexture = NULL;
	SDL_DestroyTexture( gPacmanTexture );
	gPacmanTexture = NULL;
	SDL_DestroyTexture( gGrassTexture );
	gGrassTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


SDL_Texture *loadTexture(std::string path)
{
    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void frender(){
	Pacman->update();

    SDL_Rect fillRect = { Pacman->x, Pacman->y, Pacman->cellWidth, Pacman->cellHeight };
	if (Pacman->updateAngle() == 180){
		SDL_RenderCopyEx(gRenderer, gPacmanTexture, NULL, &fillRect, 0 , NULL,SDL_FLIP_HORIZONTAL);
	}
	else SDL_RenderCopyEx(gRenderer, gPacmanTexture, NULL, &fillRect, Pacman->updateAngle() , NULL,SDL_FLIP_NONE );
}

void updateScreen(){
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			SDL_Rect fillRect = { j*cellWidth, i*cellHeight, cellWidth, cellHeight };
			if(maze[i][j]==1){
				SDL_RenderCopy(gRenderer, gWallTexture, NULL, &fillRect);
			}
			else{
				SDL_RenderCopy(gRenderer, gGrassTexture, NULL, &fillRect);
			}
		}
	}
	frender();
}


void handleEvent(SDL_Event* event){
    Pacman->HandleEvent(event);
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
