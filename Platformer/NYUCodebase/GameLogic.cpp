//
//  GameLogic.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 4/16/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include "GameLogic.h"

GameLogic::GameLogic() {
    // height, width, xpos, ypos, xvel, yvel, xaccel, yaccel, gravity, static
    player = new Entity(0.4f, 0.4f, 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 9.81f, false);
    playerTexture = TextureHandler::loadTexture(RESOURCE_FOLDER"p1_stand.png");
    player->setSprite(playerTexture, 1.0f, 1.0f, player->getWidth(), player->getHeight(), 0.5f);
    player->setLocation(1.0f, 0.0f);
    createMap();
}

void GameLogic::createMap() {
    groundTexture = TextureHandler::loadTexture(RESOURCE_FOLDER"grassCenter.png");
    groundGrassTexture = TextureHandler::loadTexture(RESOURCE_FOLDER"grassMid.png");
    leftGrassTexture = TextureHandler::loadTexture(RESOURCE_FOLDER"grassLeft.png");
    rightGrassTexture = TextureHandler::loadTexture(RESOURCE_FOLDER"grassRight.png");
    blockTextureOn = TextureHandler::loadTexture(RESOURCE_FOLDER"boxItemAlt.png");
    blockTextureOff = TextureHandler::loadTexture(RESOURCE_FOLDER"boxItemAlt_disabled.png");
    unsigned int ld[LEVEL_HEIGHT][LEVEL_WIDTH] = {
        {120,0,0,0,0,0,0,0,0,0,0,120},
        {177,0,0,0,0,0,0,0,0,0,0,177},
        {177,0,0,0,0,0,0,0,0,0,0,177},
        {177,0,0,0,0,0,0,0,0,0,0,177},
        {177,0,0,0,1,0,0,0,0,0,0,177},
        {177,0,0,0,0,0,0,0,0,0,0,177},
        {177,0,0,0,0,0,0,134,106,0,0,177},
        {177,120,120,120,120,120,120,177,177,120,120,177},
        {177,177,177,177,177,177,177,177,177,177,177,177}
    };
    for (int i = 0; i < LEVEL_HEIGHT; ++i) {
        for (int j = 0; j < LEVEL_WIDTH; j++) {
            levelData[i][j] = ld[i][j];
        }
    }
    
    for (int i = 0; i < LEVEL_HEIGHT; ++i) {
        for (int j = 0; j < LEVEL_WIDTH; j++) {
            Ground* g = new Ground(((TILE_SIZE*j) + (TILE_SIZE / 2.0f)), ((-TILE_SIZE*i) -(TILE_SIZE/2.0f)), TILE_SIZE);
            if (levelData[i][j] == 1) {
                g->setSprite(blockTextureOn, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f);
            }
            else if (levelData[i][j] == 177) {
                g->setSprite(groundTexture, 1.0f, 1.0f, 0.75f, 0.75f, 0.75f);
            }
            else if (levelData[i][j] == 120) {
                g->setSprite(groundGrassTexture, 1.0f, 1.0f, 0.75f, 0.75f, 0.75f);
            }
            else if (levelData[i][j] == 134) {
                g->setSprite(leftGrassTexture, 1.0f, 1.0f, 0.75f, 0.75f, 0.75f);
            }
            else if (levelData[i][j] == 106) {
                g->setSprite(rightGrassTexture, 1.0f, 1.0f, 0.75f, 0.75f, 0.75f);
            }
            objects.push_back(g);
        }
    }
}

void GameLogic::mapDraw(ShaderProgram& program, glm::mat4& modelMatrix) {
    int vecIndex = 0;
    for(int i = 0; i < LEVEL_HEIGHT; ++i) {
        modelMatrix = glm::mat4(1.0f);
        for(int j = 0; j < LEVEL_WIDTH; ++j) {
            if(levelData[i][j] == 0) {
                vecIndex++;
            }
            else {
                modelMatrix = glm::translate(modelMatrix, glm::vec3(objects[vecIndex]->getXPos(), objects[vecIndex]->getYPos(), 0.0f));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(objects[vecIndex]->getWidth(), objects[vecIndex]->getHeight(), 0.0f));
                objects[vecIndex]->getSprite()->Draw(program, modelMatrix, objects[vecIndex]->getSprite()->getTID(), objects[vecIndex]->getXPos(), objects[vecIndex]->getYPos());
                vecIndex++;
            }
        }
    }
}

void GameLogic::gameDraw(ShaderProgram& program, glm::mat4& modelMatrix) {
    player->draw(program, modelMatrix);
    mapDraw(program, modelMatrix);
}

void GameLogic::gameUpdate(float elapsedTime, ShaderProgram& program, glm::mat4& viewMatrix) {
    player->update(elapsedTime);
    collisionChecker();
    camera(program, viewMatrix);
}

void GameLogic::makePlayerMove(bool dir) {
    if(dir) { player->move(15.0f); }
    else { player->move(-15.0f); }
}

void GameLogic::makePlayerJump() { player->jump(); }

void GameLogic::camera(ShaderProgram& program, glm::mat4& viewMatrix) {
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(-player->getXPos(), -player->getYPos(), 0.0f));
    program.SetViewMatrix(viewMatrix);
}

void GameLogic::collisionChecker() {
    int vecInd = 0;
    for (int i = 0; i < LEVEL_HEIGHT; ++i) {
        for (int j = 0; j < LEVEL_WIDTH; ++j) {
            int gridX = (int)(player->getXPos() / TILE_SIZE);
            int gridY = (int)((player->getYPos() - (player->getHeight()/2)) / -TILE_SIZE);
            if(levelData[gridY][gridX] != 0) {
                player->setLocation(player->getXPos(), player->getYPos()+0.05f);
                player->madeCollision('d');
                vecInd++;
            }
            else {
                vecInd++;
            }
        }
    }
}
