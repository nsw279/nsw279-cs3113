#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TITLE 0
#define IN_GAME 1
#define GAME_OVER 2
#define WIN 3

#include "Entity.h"
#include "Bullet.h"
#include "SpriteSheet.h"
#include "GameState.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

// i can't do this

float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

GLuint LoadTexture(const char *filePath) {
    int w, h, comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    
    if(image == NULL) {
        std::cout << "Unable to load image\n";
        assert(false);
    }
    
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(image);
    return retTexture;
}

void DrawText(ShaderProgram& program, glm::mat4& modelMatrix, GLuint font, std::string text, float size, float space) {
    float textureSize = 1.0 / 16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for (int i = 0; i < text.size(); i++) {
        int spriteIndex = (int)text[i];
        float textureX = (float)(spriteIndex % 16) / 16.0f;
        float textureY = (float)(spriteIndex / 16) / 16.0f;
        vertexData.insert(vertexData.end(), { ((size + space) * i) + (-0.5f * size), 0.5f * size, ((size + space) * i) + (-0.5f * size), -0.5f * size, ((size + space) * i) + (0.5f * size), 0.5f * size, ((size + space) * i) + (0.5f * size), -0.5f * size, ((size + space) * i) + (0.5f * size), 0.5f * size, ((size + space) * i) + (-0.5f * size), -0.5f * size });
        texCoordData.insert(texCoordData.end(), { textureX, textureY, textureX, textureY + textureSize, textureX + textureSize, textureY, textureX + textureSize, textureY + textureSize, textureX + textureSize, textureY, textureX, textureY + textureSize });
    }
    glUseProgram(program.programID);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    program.SetModelMatrix(modelMatrix);
    glBindTexture(GL_TEXTURE_2D, font);
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6.0f);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Space Invaders Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
  
    
    glViewport(0, 0, 800, 600);
    
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    glm::mat4 viewMatrix(1.0f);
    glm::mat4 projectionMatrix(1.0f);
    glm::mat4 modelMatrix(1.0f);
    
    projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    std::string titleMessage = "Press Space to Start; Left and Right Arrows to Move and Space to Shoot";
    
    GLuint mainFont = LoadTexture("/Users/noahweitz/Documents/Documents - Noah Weitz's MacBook Pro/School/Game Programming/Personal Repo/nsw279-cs3113/Space Invaders/font1.png");
    
    GLuint greenShip = LoadTexture("/Users/noahweitz/Documents/Documents - Noah Weitz's MacBook Pro/School/Game Programming/Personal Repo/nsw279-cs3113/Space Invaders/Space shooter assets (300 assets)/PNG/playerShip1_green.png");
    GLuint enemyShip = LoadTexture("/Users/noahweitz/Documents/Documents - Noah Weitz's MacBook Pro/School/Game Programming/Personal Repo/nsw279-cs3113/Space Invaders/Space shooter assets (300 assets)/PNG/Enemies/enemyRed1.png");
    
    GameState state;
    Entity ship(true, greenShip);
    Entity* enemy[] = { new Entity(false, enemyShip), new Entity(false, enemyShip), new Entity(false, enemyShip), new Entity(false, enemyShip), new Entity(false, enemyShip), new Entity(false, enemyShip), new Entity(false, enemyShip), new Entity(false, enemyShip)};
    Bullet playerBullet(true);
    Bullet enemyBullet(false);
    
    state.setScreen(TITLE);

#ifdef _WINDOWS
    glewInit();
#endif
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    if(state.getScreen() == TITLE) {
                        state.setScreen(IN_GAME);
                    }
                    else if (state.getScreen() == IN_GAME) {
                        std::cout << "Player shot a bullet" << std::endl;
                        ship.shootBullet(true, playerBullet);
                    }
                    else if (state.getScreen() == GAME_OVER || state.getScreen() == WIN) {
                        state.setScreen(TITLE);
                    }
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    if (state.getScreen() == IN_GAME) { ship.moveX(-0.1f); std::cout << "Player moved left" << std::endl; }
                }
                else if(event.key.keysym.sym == SDLK_RIGHT) {
                    if (state.getScreen() == IN_GAME) { ship.moveX(0.1f); std::cout << "Player moved right" << std::endl;}
                }
            }
            
        }
        
        program.SetViewMatrix(viewMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetModelMatrix(modelMatrix);
        
        int currentSt = state.getScreen();
        switch (currentSt) {
            case TITLE:
                // Draw title screen
                std::cout << "State is title screen" << std::endl;
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f, 1.0f, 0.0f));
                DrawText(program, modelMatrix, mainFont, titleMessage, 12, 1);
                modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(ship.getX(), ship.getY(), 0.0f));
                ship.draw(program, modelMatrix);
                break;
            case IN_GAME:
                // Draw gameplay
                std::cout << "State is in game" << std::endl;
                if(ship.colliding(enemyBullet.getX(), enemyBullet.getY(), enemyBullet.getW(), enemyBullet.getH())) {
                    ship.destroy();
                    if (state.getLives() == 0) {
                        state.setScreen(2);
                    }
                    else {
                        state.subtractLife();
                    }
                }
                for (auto e: enemy) {
                    if (e->colliding(playerBullet.getX(), playerBullet.getY(), playerBullet.getW(), playerBullet.getH())) {
                        if (state.getEnemies() == 0) {
                            state.setScreen(3);
                        }
                        else {
                            e->destroy();
                            state.subtractEnemy();
                        }
                    }
                }
                break;
            case GAME_OVER:
                DrawText(program, modelMatrix, mainFont, "GAME OVER", 12, 1);
                break;
            case WIN:
                DrawText(program, modelMatrix, mainFont, "YOU WON!", 3, 1);
                break;
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
