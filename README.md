## Installing SDL libraries on Linux:

1. To install sdl2  
`sudo apt install libsdl2-dev libsdl2-2.0-0 -y;`

2. To install sdl image  
`sudo apt install libjpeg-dev libwebp-dev libtiff5-dev libsdl2-image-dev libsdl2-image-2.0-0 -y;`

3. To install sdl mixer  
`sudo apt install libmikmod-dev libfishsound1-dev libsmpeg-dev liboggz2-dev libflac-dev libfluidsynth-dev libsdl2-mixer-dev libsdl2-mixer-2.0-0 -y;`

    **Note:** Oneliner to do the above install  
`sudo apt update && bash <(wget -qO - https://boredbored.net/sdl2-install)`

	**Note:** Our operating system is Linux 64-bit (Distribution- Ubuntu 20.04)

## How to run the game:

1. Clone this repository using `git clone https://github.com/guptaaniket261/COP290Assignment2.git` or download as Zip and extract.
2. Change the current directry to move into this folder in the terminal.
3. Make sure you have installed SDL using the above instructions, you must be using a Linux OS.
4. Run the `make` command. This should produce a binary executable name "game" in the same directory.
5. Now run the `./game` command.
6. For further instructions on how to play the game, etc refer to [Presentation.](https://docs.google.com/presentation/d/1nFNLSMxkS-R8G_cky4LpqTDy-qXT2I7-DQGiqBkEyBg/edit?usp=sharing)

## Online resources used:

1. For studying SDL and it's functionalities we have reffered to [Lazyfoo website](https://lazyfoo.net/tutorials/SDL/index.php).  
We have also reffered to the [official documentation of SDL](https://wiki.libsdl.org/)
2. We have taken the idea of algorithm of Bot zombie movement from [this video.](https://www.youtube.com/watch?v=ataGotQ7ir8&list=LL&index=6)
3. For taking basic idea of Maze formation we have reffered [wikipedia.](https://en.wikipedia.org/wiki/Maze_generation_algorithm)  
Further making a maze with no dead ends was done by us.
4. For installing SDL we reffered [this page.](https://gist.github.com/BoredBored/3187339a99f7786c25075d4d9c80fad5)