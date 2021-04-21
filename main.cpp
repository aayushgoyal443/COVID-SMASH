#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 720*1.5;
const int SCREEN_HEIGHT = 540*1.5;

// The enum values for the start menu
enum KeyPress_start
{
    KEY_MENU,
    KEY_1P,
    KEY_2P,
    KEY_HELP,
    KEY_CREDITS,
    KEY_TOTAL
};

// For checking the position of mouse on start menu
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

int main(int argc, char *args[])
{
    //Start up SDL and create window
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
                        gameCurrentTexture = gameKeyPressTextures[KEY_1P];
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
                if (e.button.button == SDL_BUTTON_LEFT)
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

    //Free resources and close SDL
    close();

    return 0;
}