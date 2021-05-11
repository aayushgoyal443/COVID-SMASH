#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>


using namespace std;


bool gameRunning = false;
bool gameServer = false;
bool gameClient  = false;
const int SCREEN_WIDTH = 720*1.5;
const int SCREEN_HEIGHT = 540*1.5;
const int cellHeight =24;
const int cellWidth = 24;

enum KeyPress_start
{
    KEY_MENU,
    KEY_1P,
    KEY_2P,
    KEY_HELP,
    KEY_CREDITS,
    KEY_TOTAL
};

int BOT_alive=1;
int BOT2_alive =1;
int BOT3_alive=2;
int zombie_alive=1;
int pacmanLives =5;
int eggsComplete = 0;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window *gameWindow = NULL;

//The window renderer
SDL_Renderer *gameRenderer = NULL;

//The images that correspond to a keypress
SDL_Texture *gameKeyPressTextures[KEY_TOTAL];

//Current displayed image
SDL_Texture *gameCurrentTexture = NULL;

SDL_Texture* gWallTexture = NULL;
SDL_Texture* gGrassTexture = NULL;
SDL_Texture* gPacmanTexture = NULL;
SDL_Texture* gZombieTexture = NULL;
SDL_Texture* eggTexture = NULL;
SDL_Texture* vaccineTexture = NULL;
SDL_Texture* deadZombieTexture =NULL;

pacman* Pacman = NULL;
bot* BOT = NULL;    // this will directly chase the Pacman 
bot* BOT2 = NULL;   // this will directly chase the Pacman from 180 degree behind
bot* BOT3 = NULL;   // this will chase the pacman from Front, 4 rows change scheme.






KeyPress_start check_position(int x, int y)
{
    int x_start = 241;
    int x_width = 237; // width of button is
    int y_width = 44;
    int y_single = 171;
    int y_double = 232;
    int y_help = 293;
    int y_credits = 356;
    x_start *= 1.5;
    x_width *= 1.5;
    y_width *= 1.5;
    y_single *= 1.5;
    y_double *= 1.5;
    y_help *= 1.5;
    y_credits *= 1.5;

    if (x >= x_start && x <= (x_start + x_width))
    {
        if (y >= y_single && y <= (y_single + y_width))
        {
            // cout << KEY_1P;
            return KEY_1P;
        }
        else if (y >= y_double && y <= (y_double + y_width))
        {
            // cout << KEY_2P;
            return KEY_2P;
        }
        else if (y >= y_help && y <= (y_help + y_width))
        {
            // cout << KEY_HELP;
            return KEY_HELP;
        }
        else if (y >= y_credits && y <= (y_credits + y_width))
        {
            // cout << KEY_CREDITS;
            return KEY_CREDITS;
        }
        else
        {
            // cout << KEY_MENU;
            return KEY_MENU;
        }
    }
    // cout << KEY_MENU;
    return KEY_MENU;
}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gameWindow = SDL_CreateWindow("PACMAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gameWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gameRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Start menu surface
    gameKeyPressTextures[KEY_MENU] = loadTexture("Resources/start_menu.png");
    if (gameKeyPressTextures[KEY_MENU] == NULL)
    {
        printf("Failed to load default image!\n");
        success = false;
    }

    //Load 1 Player surface
    gameKeyPressTextures[KEY_1P] = loadTexture("Resources/1_player.png");
    if (gameKeyPressTextures[KEY_1P] == NULL)
    {
        printf("Failed to load 1 Player image!\n");
        success = false;
    }

    //Load 2 Player surface
    gameKeyPressTextures[KEY_2P] = loadTexture("Resources/2_player.png");
    if (gameKeyPressTextures[KEY_2P] == NULL)
    {
        printf("Failed to load 2 Player image!\n");
        success = false;
    }

    //Load Help surface
    gameKeyPressTextures[KEY_HELP] = loadTexture("Resources/help.png");
    if (gameKeyPressTextures[KEY_HELP] == NULL)
    {
        printf("Failed to load help image!\n");
        success = false;
    }

    //Load Credits surface
    gameKeyPressTextures[KEY_CREDITS] = loadTexture("Resources/credits.png");
    if (gameKeyPressTextures[KEY_CREDITS] == NULL)
    {
        printf("Failed to load credits image!\n");
        success = false;
    }

    gPacmanTexture = loadTexture("Resources/pacman.png");
    if (gPacmanTexture == NULL)
    {
        printf("Failed to load Pacman image!\n");
        success = false;
    }

    gZombieTexture = loadTexture("Resources/zombie.png");
    if (gZombieTexture == NULL)
    {
        printf("Failed to load Zombie image!\n");
        success = false;
    }

    deadZombieTexture = loadTexture("Resources/dead_zombie.png");
    if (deadZombieTexture == NULL)
    {
        printf("Failed to load Dead Zombie image!\n");
        success = false;
    }

	gWallTexture = loadTexture("Resources/wall.png");
    if (gWallTexture == NULL)
    {
        printf("Failed to load wall image!\n");
        success = false;
    }

	gGrassTexture = loadTexture("Resources/grass.png");
    if (gGrassTexture == NULL)
    {
        printf("Failed to load grass image!\n");
        success = false;
    }

    eggTexture = loadTexture("Resources/egg.png");
    if (eggTexture == NULL)
    {
        printf("Failed to load egg image!\n");
        success = false;
    }

    vaccineTexture = loadTexture("Resources/vaccine.png");
    if (vaccineTexture == NULL)
    {
        printf("Failed to load vaccine image!\n");
        success = false;
    }
    return success;
}

void close()
{
    //Free loaded image
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        SDL_DestroyTexture(gameKeyPressTextures[i]);
        gameKeyPressTextures[i] = NULL;
    }
    SDL_DestroyTexture(gameCurrentTexture);
    gameCurrentTexture = NULL;

    SDL_DestroyTexture( gWallTexture );
	gWallTexture = NULL;
	SDL_DestroyTexture( gPacmanTexture );
	gPacmanTexture = NULL;
	SDL_DestroyTexture( gGrassTexture );
	gGrassTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindow);
    gameWindow = NULL;
    gameRenderer = NULL;

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
        newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}