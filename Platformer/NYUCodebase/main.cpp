//#ifdef _WINDOWS
//#include <GL/glew.h>
//#endif
//#include <SDL.h>
//#define GL_GLEXT_PROTOTYPES 1
//#include <SDL_opengl.h>
//#include <SDL_image.h>
//#include "ShaderProgram.h"
//#include "glm/mat4x4.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//
//#ifdef _WINDOWS
//#define RESOURCE_FOLDER ""
//#else
//#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
//#endif

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

#include "Entity.h"
#include "GameLogic.h"
#include "Graphical.h"

SDL_Window* displayWindow;

void GeneralRender(ShaderProgram& program, glm::mat4& modelMatrix, GameLogic& runner) {
    runner.gameDraw(program, modelMatrix);
}

void GeneralUpdate(float elapsed, GameLogic& runner, ShaderProgram& program, glm::mat4& viewMatrix) {
    runner.gameUpdate(elapsed, program, viewMatrix);
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Platformer Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 800, 600);
    
    float lastFrameTicks = 0.0f;
    
    ShaderProgram program;
//    program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    GameLogic runner;
    
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) { runner.makePlayerJump(); }
            }
            if (keys[SDL_SCANCODE_LEFT]) { runner.makePlayerMove(false); }
            else if (keys[SDL_SCANCODE_RIGHT]) { runner.makePlayerMove(true); }
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        program.SetViewMatrix(viewMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetModelMatrix(modelMatrix);

        GeneralRender(program, modelMatrix, runner);
        GeneralUpdate(elapsed, runner, program, viewMatrix);
        // std::cout << "elapsed: " << elapsed << std::endl;
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
