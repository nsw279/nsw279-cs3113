//
//  GameLogic.h
//  NYUCodebase
//
//  Created by Noah Weitz on 4/16/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#ifndef GameLogic_h
#define GameLogic_h

#define LEVEL_HEIGHT 9
#define LEVEL_WIDTH 12

#define TILE_SIZE 0.7f

#include "Entity.h"
#include "Graphical.h"

class Entity;

class GameLogic {
private:
    int mapHeight;
    int mapWidth;
    unsigned int levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
    std::vector<Ground*> objects;
    Entity* player;
    GLuint playerTexture;
    GLuint groundTexture;
    GLuint groundGrassTexture;
    GLuint leftGrassTexture;
    GLuint rightGrassTexture;
    GLuint blockTextureOn;
    GLuint blockTextureOff;
public:
    GameLogic();
    
    void createMap();
    void gameDraw(ShaderProgram& program, glm::mat4& modelMatrix);
    void mapDraw(ShaderProgram& program, glm::mat4& modelMatrix);
    void gameUpdate(float elapsedTime, ShaderProgram& program, glm::mat4& viewMatrix);
    void collisionChecker();
    void collisionHandler();
    void makePlayerMove(bool dir);
    void makePlayerJump();
    void camera(ShaderProgram& program, glm::mat4& viewMatrix);
    // void reset();

};

#endif /* GameLogic_h */
