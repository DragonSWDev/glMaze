#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "ShaderManager.hpp"
#include "MazeGenerator.hpp"

int windowWidth, windowHeight, mazeSize;
bool enableCollisions, setFullscreen;

//Check collision between point (camera) and cube (maze field)
//For cube add some margin (0.2f) to avoid watching through walls
bool collide(float pointX, float pointY, float wallX, float wallY)
{
    if (pointX >= wallX - 0.7f && pointX <= wallX + 0.7f &&
        pointY >= wallY - 0.7f && pointY <= wallY + 0.7f)
            return true;

    return false;
}

//Get distance between to points in 3D space
float getDistance(glm::vec3 pointA, glm::vec3 pointB)
{
    return glm::length(pointB - pointA);
}

//Parse command line arguments and set correct values (fallback to default values if they are wrong)
void parseArguments(std::vector<std::string> arguments)
{
    int width = 0;
    int height = 0;

    for (auto& argument : arguments)
    {
        //Arguments always begins with '-'
        if (argument[0] != '-')
            continue;

        if (argument.find("-width=") != std::string::npos && argument.size() > 7)
        {
            std::string widthstr = argument.substr(7, argument.size());

            try
            {
                width = std::stoi(widthstr);
            }
            catch(...)
            {
                std::cerr << "Wrong width value!" << std::endl;
                width = 0;
            }
        }

        if (argument.find("-height=") != std::string::npos && argument.size() > 8)
        {
            std::string heightstr = argument.substr(8, argument.size());

            try
            {
                height = std::stoi(heightstr);
            }
            catch(...)
            {
                std::cerr << "Wrong height value!" << std::endl;
                height = 0;
            }
        }

        if (argument.find("-size=") != std::string::npos && argument.size() > 6)
        {
            std::string sizestr = argument.substr(6, argument.size());
            
            int size;

            try
            {
                size = std::stoi(sizestr);
            }
            catch(...)
            {
                std::cerr << "Wrong size value!" << std::endl;
                size = 0;
            }

            if (size < 10 || size > 1000)
                mazeSize = 20;
            else
                mazeSize = size;
        }

        if (argument.find("-disable-collisions") != std::string::npos)
            enableCollisions = false;

        if (argument.find("-fullscreen") != std::string::npos)
            setFullscreen = true;
    }

    //Set correct resolution (only for window, full screen uses native resolution)
    if (width < 100 || width > 4096 || height < 100 || height > 2160)
    {
        width = 800;
        height = 600;
    }

    if (height > width)
        height = width;

    windowWidth = width;
    windowHeight = height;
}

int main(int argc, char* argv[])
{
    windowWidth = 800;
    windowHeight = 600;
    mazeSize = 20;
    enableCollisions = true;
    setFullscreen = false;

    std::vector<std::string> arguments;

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
            arguments.push_back(argv[i]);

        parseArguments(arguments);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL init failed! Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    //Fullscreen requested, get display mode
    if (setFullscreen)
    {
        SDL_DisplayMode mode;

        if (SDL_GetDesktopDisplayMode(0, &mode) != 0) 
        {
            std::cerr << "Failed to get current display mode!" << SDL_GetError() << std::endl;
            return -1;
        }

        windowWidth = mode.w;
        windowHeight = mode.h;
    }

    //Print selected options
    std::cout << "Selected options: " << std::endl;
    std::cout << "Resolution: " << windowWidth << "x" << windowHeight << " ";
    std::cout << (setFullscreen ? "fullscreen" : "windowed") << std::endl;
    std::cout << "Maze size: " << mazeSize << std::endl;
    std::cout << (enableCollisions ? "Collisions enabled" : "Collisions disabled") << std::endl << std::endl;

    IMG_Init(IMG_INIT_PNG);

    srand(time(0));

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //Set OpenGL context to OpenGL 3.3 Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window *mainWindow = SDL_CreateWindow("OpenGL Maze", 0, 0, windowWidth, windowHeight, SDL_WINDOW_OPENGL);

    if (setFullscreen)
    {
        SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        SDL_ShowCursor(SDL_DISABLE);
    }

    if (mainWindow == nullptr)
    {
        std::cout << "Failed creating SDL Window!" << std::endl;
        return -1;
    }
    
    SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow);

    if (glContext == nullptr) 
    {
        std::cout << "Error creating OpenGL context!" << std::endl;
        return -1;
    }

    //Init GLAD
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed initializing GLAD!" << std::endl;
        return -1;
    }

    std::cout << "OpenGL initialized" << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

    //Setup OpenGL viewport
    glViewport(0, 0, windowWidth, windowHeight);

    //Enable depth test
    glEnable(GL_DEPTH_TEST);

    //Enable face culling
    glEnable(GL_CULL_FACE);

    //Cube vertices
    float wallVertices[] = {
        //Vertex positions      //Texture coordinates   //Normal vector
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f,             0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f,             0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,             0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f,             0.0f, 0.0f, 1.0f
    };

    //VBO indexes
    unsigned int wallIndices[] = {  
        0, 1, 3, //First triangle
        1, 2, 3 //Second triangle
    };

    ShaderManager shader;
    
    if (!shader.loadShaders("./shaders/vertexshader.vert", "./shaders/fragmentshader.frag"))
    {
        std::cerr << "Shader loading or compilation failed!" << std::endl;
        return -1;
    }

    //Loading wall texture
    SDL_Surface* wallImage = IMG_Load("./assets/wall.png");

    if (wallImage == nullptr)
    {
        std::cerr << "Failed loading wall.png texture!" << std::endl;
        return -1;
    }

    //Loading floor texture
    SDL_Surface* floorImage = IMG_Load("./assets/floor.png");

    if (floorImage == nullptr)
    {
        std::cerr << "Failed loading floor.png texture!" << std::endl;
        return -1;
    }

    SDL_Surface* ceilingImage = IMG_Load("./assets/ceiling.png");

    if (ceilingImage == nullptr)
    {
        std::cerr << "Failed loading ceiling.png texture!" << std::endl;
        return -1;
    }

    GLuint vertexBufferObject, vertexArrayObject, elementArrayBuffer;

    glGenVertexArrays(1, &vertexArrayObject); //Generate vertex buffer, element buffer and vertex array objects
    glGenBuffers(1, &vertexBufferObject);
    glGenBuffers(1, &elementArrayBuffer);

    glBindVertexArray(vertexArrayObject); //Use vertex array object

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW); //Move vertices table to vertex buffer object

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer); //Move indices table to element buffer object
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0); //Position attribute
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float))); //Texture attribute
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(float))); //Normal attribute
    glEnableVertexAttribArray(2);

    //Create OpenGL textures
    GLuint mazeTextures[3];
    glGenTextures(3, mazeTextures);

    //Setup wall texture
    glBindTexture(GL_TEXTURE_2D, mazeTextures[0]);

    //Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wallImage->w, wallImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, wallImage->pixels); //Get texture data from SDL surface
    
    glGenerateMipmap(GL_TEXTURE_2D);

    //Setup floor texture
    glBindTexture(GL_TEXTURE_2D, mazeTextures[1]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floorImage->w, floorImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, floorImage->pixels); //Get texture data from SDL surface
    
    glGenerateMipmap(GL_TEXTURE_2D);

    //Setup ceiling texture
    glBindTexture(GL_TEXTURE_2D, mazeTextures[2]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ceilingImage->w, ceilingImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ceilingImage->pixels); //Get texture data from SDL surface
    
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Set model, view and projection matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    //Generate maze and get pointer to array
    MazeGenerator maze(mazeSize);
    maze.generateMaze();
    bool** mazeArray = maze.getMazeArray();

    //Setup camera matrices
    glm::vec3 cameraPosition   = glm::vec3(maze.getStartX()*1.0f, 0.0f, maze.getStartY()*1.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    float yawAngle = -90.0f;

    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

    //Enable vsync
    SDL_GL_SetSwapInterval(1);

    SDL_Event windowEvent;
    bool isRunning = true;

    float deltaTime, lastFrame, currentFrame, cameraSpeed, movementSpeed;
    lastFrame = SDL_GetTicks()*0.001f;
    deltaTime = lastFrame;
    bool isCollision = false;

    glm::vec3 lastPos = cameraPosition;

    while (isRunning)
    {
        while (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
                isRunning = false;
        } 

        //Setup delta time and movement/camera speed
        cameraSpeed = 90.0f * deltaTime;
        movementSpeed = 2.5f * deltaTime;
        currentFrame = SDL_GetTicks()*0.001f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //If there is collision then change camera position to last position and put down collison flag
        if (isCollision)
        {
            cameraPosition = lastPos;
            isCollision = false;
        }

        //Get SDL keyboard state
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        
        //Process input
        if (state[SDL_SCANCODE_UP])
        { 
            lastPos = cameraPosition;
            cameraPosition += movementSpeed * cameraFront;
        }

        if (state[SDL_SCANCODE_DOWN])
        {
            lastPos = cameraPosition;
            cameraPosition -= movementSpeed * cameraFront;
        }

        if (state[SDL_SCANCODE_LEFT])
            yawAngle -= cameraSpeed;
        

        if (state[SDL_SCANCODE_RIGHT])
            yawAngle += cameraSpeed;

        if (state[SDL_SCANCODE_N]) //Generate new maze
        {
            cameraPosition = glm::vec3(maze.getStartX()*1.0f, 0.0f, maze.getStartY()*1.0f);
            maze.generateMaze();
        }

        if (state[SDL_SCANCODE_ESCAPE])
            isRunning = false;

        //Setup camera
        cameraFront.x = cos(glm::radians(yawAngle));
        cameraFront.y = sin(glm::radians(0.0f)); 
        cameraFront.z = sin(glm::radians(yawAngle));        

        //Begin drawing
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Bind wall texture
        glBindTexture(GL_TEXTURE_2D, mazeTextures[0]);

        //Use shader
        shader.useShader();
        
        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        shader.setUniformMatrix4fv("view", view);

        shader.setUniformMatrix4fv("projection", projection);

        shader.setUniformVec3fv("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setUniformVec3fv("lightVector", cameraPosition);

        glBindVertexArray(vertexArrayObject);

        //Draw walls, floor and ceiling
        //It should draw only visible walls so we are checking if we are on filled field and then check all four neighbours
        //Wall is only visible if neighbour is empty field, if it is we move and rotate plane in the right position to make cube
        //Same goes for floor and ceiling
        //Also check for collision
        for (unsigned int i = 1; i < maze.getMazeSize() - 1; i++) //Exclude margin
        {
            for (unsigned int j = 1; j < maze.getMazeSize() - 1; j++)
            {
                //Check for collision
                if (enableCollisions && mazeArray[i][j] && collide(cameraPosition.x, cameraPosition.z, j*1.0f, i*1.0f))
                    isCollision = true;

                //Don't draw anything that is further than 20
                //If it is then it's pretty big chance it won't be visible so it's very simple optimization
                //With big mazes there is chance that end of long hall will be invisible because it's further than 20 but it most cases it works fine
                //It's still better than drawing everything because performance won't drop in big mazes
                if (getDistance(cameraPosition, glm::vec3(j*1.0f, 0.0f, i*1.0f)) > 20.0f)
                {
                    continue;
                }

                //Bind wall texture
                glBindTexture(GL_TEXTURE_2D, mazeTextures[0]);

                if (mazeArray[i][j] && !mazeArray[i][j-1]) //Left
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(j*1.0f, 0.0f, i*1.0f)); //Move to right position
                    model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f)); //Move left a bit
                    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotate by 90 degrees around Y

                    shader.setUniformMatrix4fv("model", model);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
                
                if (!mazeArray[i][j+1] && mazeArray[i][j]) //Right
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(j*1.0f, 0.0f, i*1.0f)); //Move to right position
                    model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); //Move right a bit
                    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotate by 90 degrees around Y

                    shader.setUniformMatrix4fv("model", model);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
                
                if (!mazeArray[i-1][j] && mazeArray[i][j]) //Front
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(j*1.0f, 0.0f, i*1.0f)); //Move to right position
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f)); //Move front a bit

                    shader.setUniformMatrix4fv("model", model);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
                
                if (!mazeArray[i+1][j] && mazeArray[i][j]) //Back
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(j*1.0f, 0.0f, i*1.0f)); //Move to right position
                    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f)); //Move back a bit
                    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                    shader.setUniformMatrix4fv("model", model);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }

                //Draw floor and ceiling
                if (!mazeArray[i][j]) //Draw floor/ceiling tile only under empty fields
                {
                    //Floor
                    glBindTexture(GL_TEXTURE_2D, mazeTextures[1]);

                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(j*1.0f, 0.0f, i*1.0f));
                    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
                    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

                    shader.setUniformMatrix4fv("model", model);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    //Ceiling
                    glBindTexture(GL_TEXTURE_2D, mazeTextures[2]);

                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(j*1.0f, 0.0f, i*1.0f));
                    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
                    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

                    shader.setUniformMatrix4fv("model", model);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
        }

        //Swap buffers and end drawing
        SDL_GL_SwapWindow(mainWindow);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(mainWindow);
    IMG_Quit();
    SDL_Quit();

    return 0;
}