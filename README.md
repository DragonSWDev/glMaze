# glMaze
This is a simple modern OpenGL game that I wrote after completing first step of the learnopengl.com online book. It's a very simple 3D game where player starts in random position and needs to find exit in randomly generated (Depth-first search and recursive division algorithms) perfect maze. It's implemented using OpenGL, SDL2 and GLAD libraries.

<span style="display:block;text-align:center">![Screenshot](./doc/screenshot.png)

## Installation guide

### 1. Install SDL2 with SDL_image and OpenGL development libraries

### 2. Clone repository with command
```console
$ git clone https://github.com/DragonSWDev/glMaze && cd glMaze
```

### 3. Build
```console
make
```

### 4. Run
```console
./glmaze
```
#### Note: "assets" and "shaders" directory needs to be present in same directory as exectuable.

## Manual
### Configuration options
Configurations is specified by command line arguments. Options can be specified in any order and count.

**-width=value** - Window width

**-height=value** - Window height
#### Note: This values are respected only if game works in windowed mode. In fullscreen mode game always set desktop resolution. With custom window size both values (width and height) needs to be specified and height can't be bigger than width. Default size is 800x600.

**-size=value** - Maze size (Min is 10, max is 100000, default 20). 
#### Note: For big mazes (more than 1000) it's better to use RD generator because DFS is pretty slow and generating big mazes will last long time even on fast CPU. Big mazes will also consume more memory. For 100000 size (RD generator) application consumes over 9 GiB of RAM.

**-disable-collisions** - Disable collisions

**-fullscreen** - Run in fullscreen mode

**-generator=value** - Select maze generator: "RD" for recursive division and "DFS" for depth-first search. Default is "RD".

**-seed=value** - Generator seed

### Controls
Camera is controlled by keyboard, where Up and Down arrows are for moving forward/backward and Left and Right arrows are for rotating camera left/right. Key Escape will close the game.

## License
glMaze is distributed under MIT License (see LICENSE file for more informations). Project is using SDL2, SDL_image and GLAD. For informations about these libraries licensing see their respective websites. Assets are made by me and distributed under CC BY 4.0 license. See details here: https://creativecommons.org/licenses/by/4.0/
