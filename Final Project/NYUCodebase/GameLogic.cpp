//
//  GameLogic.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 4/16/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include "GameLogic.h"

GameLogic::GameLogic() {
    whichScreen = TITLE;
    score = 0;
    lives = 3;
    returnTimer = 0.0f;
    won = false;
    kamiIsComing = false;
    //player parameters: height, width, xpos, ypos, xvel, yvel, xaccel, yaccel, gravity, static
    player = new Entity(0.4f, 0.4f, -1.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    player->setPlayer();
    font1 = TextureHandler::loadTexture(RESOURCE_FOLDER"pixel_font.png");
    playerTexture = TextureHandler::loadTexture(RESOURCE_FOLDER"playership.png");
    enemyTex1 = TextureHandler::loadTexture(RESOURCE_FOLDER"enemy1.png");
    enemyTex2 = TextureHandler::loadTexture(RESOURCE_FOLDER"enemy2.png");
    pBulletTex = TextureHandler::loadTexture(RESOURCE_FOLDER"laserGreen.png");
    eBulletTex = TextureHandler::loadTexture(RESOURCE_FOLDER"laserRed.png");
    particleEm = *new ParticleEmitter(10000);
    explosionEm = *new ExplosionEmitter(1000);
    player->setSprite(playerTexture, 1.0f, 1.0f, player->getWidth(), player->getHeight(), 0.2f);
    player->setAlive(true);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    playerExplode = Mix_LoadWAV(RESOURCE_FOLDER"youexplode.wav");
    enemyExplode = Mix_LoadWAV(RESOURCE_FOLDER"enemyexplode.wav");
    laser = Mix_LoadWAV(RESOURCE_FOLDER"laser.wav");
    titleM = Mix_LoadMUS(RESOURCE_FOLDER"menu.wav");
    gameM = Mix_LoadMUS(RESOURCE_FOLDER"battle.wav");
    goM = Mix_LoadMUS(RESOURCE_FOLDER"in-the-wreckage");
    Mix_PlayMusic(titleM, -1);
    // Music from https://gooseninja.itch.io/space-music-pack
    // Sound effects from https://phoenix1291.itch.io/sound-effects-pack-2
}

void GameLogic::mapDraw(ShaderProgram& program, glm::mat4& modelMatrix, int which) {
    
}

void GameLogic::prepareLevel(int which) {
    enemies.clear();
    bullets.clear();
    if (which == LVL1) {
        levelRemainingEnemies = 5;
        for (int i = 0; i < levelRemainingEnemies; ++i) {
            Enemy* ne = new Enemy(true);
            ne->setLocation(1.0f, 0.8f-(i*0.4));
            ne->setSprite(enemyTex1, 1.0f, 1.0f, ne->getWidth(), ne->getHeight(), 0.2f);
            ne->setEnemy();
            enemies.push_back(ne);
        }
        
    }
    else if (which == LVL2) {
        levelRemainingEnemies = 4;
        for (int i = 0; i < levelRemainingEnemies; ++i) {
            Enemy* ne = new Enemy(false);
            ne->setLocation(1.0f, 0.8f-(i*0.5));
            ne->setSprite(enemyTex2, 1.0f, 1.0f, ne->getWidth(), ne->getHeight(), 0.2f);
            enemies.push_back(ne);
        }
    }
    else if (which == LVL3) {
        levelRemainingEnemies = 8;
        for (int i = 0; i < levelRemainingEnemies; ++i) {
            if(i < levelRemainingEnemies/2){
                Enemy* ne = new Enemy(false);
                ne->setLocation(1.0f, 0.8f-(i*0.4));
                ne->setSprite(enemyTex2, 1.0f, 1.0f, ne->getWidth(), ne->getHeight(), 0.2f);
                enemies.push_back(ne);
            }
            else {
                Enemy* ne = new Enemy(true);
                ne->setLocation(1.3f, 0.8-((i/2)*0.4));
                ne->setSprite(enemyTex1, 1.0f, 1.0f, ne->getWidth(), ne->getWidth(), 0.2f);
                enemies.push_back(ne);
            }
        }
    }
    else {
        std::cerr << "Trying to access an invalid level!" << std::endl;
        exit(1);
    }
}

void GameLogic::gameDraw(ShaderProgram& program, glm::mat4& modelMatrix) {
    if(whichScreen == TITLE) { // Title Screen
        drawTitle(program, modelMatrix);
        particleEm.Render(program, modelMatrix);
    }
    else if(whichScreen == LVL1) { // Level 1
        player->draw(program, modelMatrix);
        drawLevel(program, modelMatrix, LVL1);
        particleEm.Render(program, modelMatrix);
    }
    else if(whichScreen == LVL2) { // Level 2
        player->draw(program, modelMatrix);
        drawLevel(program, modelMatrix, LVL2);
        particleEm.Render(program, modelMatrix);
    }
    else if(whichScreen == LVL3) { // Level 3
        player->draw(program, modelMatrix);
        drawLevel(program, modelMatrix, LVL3);
        particleEm.Render(program, modelMatrix);
    }
    else if(whichScreen == GAMEOVER) { // Game Over Screen
        drawGO(program, modelMatrix);
    }
    else if(whichScreen == INSTR) { // Instruction Screen
        drawInstr(program, modelMatrix);
    }
    else {
        std::cerr << "Invalid Level Number!" << std::endl;
        exit(1);
    }
    
}

void GameLogic::drawTitle(ShaderProgram& program, glm::mat4& modelMatrix) {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f,0.9f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "S-Flight", 0.1f, -0.01f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.7f,-0.8f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Press 1 to start", 0.05f, 0.01f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.7f,-0.9f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Press ESC to Quit, I for Instructions", 0.05f, 0.01f);
}

void GameLogic::drawLevel(ShaderProgram& program, glm::mat4& modelMatrix, int which) {
    std::string HUDText = "Level "+std::to_string(whichScreen)+" Lives: "+std::to_string(lives)+" Score: "+std::to_string(score);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.7f,0.9f,0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, HUDText, 0.06, 0.01f);
    
    for(Bullet* b: bullets) b->draw(program, modelMatrix);
    for(auto e: enemies) { if (e->getAlive()) { e->draw(program, modelMatrix); } }
}

void GameLogic::drawGO(ShaderProgram& program, glm::mat4& modelMatrix) {
    if(!won) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.7, 0.9f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        TextureHandler::DrawText(program, font1, "GAME OVER", 0.2, -0.01f);
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.7, 0.7f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        TextureHandler::DrawText(program, font1, "Your final score is:", 0.05, -0.01f);
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.7, 0.6f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        TextureHandler::DrawText(program, font1, std::to_string(score), 0.05, -0.01f);
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.7f,-0.9f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        TextureHandler::DrawText(program, font1, "Press ESC to Quit or T for title", 0.05f, 0.01f);
    }
    else {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.1f, 0.9f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        TextureHandler::DrawText(program, font1, "You have defeated all enemies!", 0.1, -0.01f);
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.7, 0.7f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        TextureHandler::DrawText(program, font1, "Your final score is:", 0.05, -0.01f);
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.7, 0.6f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        TextureHandler::DrawText(program, font1, std::to_string(score), 0.05, -0.01f);
        
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.7f,-0.9f, 0.0f));
        program.SetModelMatrix(modelMatrix);
        TextureHandler::DrawText(program, font1, "Press ESC to Quit or T for title", 0.05f, 0.01f);
    }
}

void GameLogic::drawInstr(ShaderProgram& program, glm::mat4& modelMatrix) {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f,0.9f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Instructions", 0.1f, -0.01f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.7f,0.7f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Press the up and down arrow keys to move", 0.06f, -0.01f);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.7f,0.6f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Press spacebar to fire", 0.06f, -0.01f);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.7f,0.5f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Press ESC at any time to quit", 0.06f, -0.01f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f,0.3f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Destroy all enemies to move to next level", 0.06f, -0.01f);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f,0.2f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "If you contact an enemy or its projectile, you lose a life", 0.06f, -0.01f);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f,0.1f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Lose all 3 lives and it's game over", 0.06f, -0.01f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.7f, -0.9f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Press 1 to return to title", 0.06f, -0.01f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.3f, -0.1f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "ENEMIES", 0.06f, -0.01f);
    
    Enemy en1;
    Enemy en2;
    
    en1.setSprite(enemyTex1, 1.0f, 1.0f, 0.4f, 0.4f, 0.2f);
    en2.setSprite(enemyTex2, 1.0f, 1.0f, 0.4f, 0.4f, 0.2f);
    
    en1.setLocation(-0.5f, -0.3f);
    en1.draw(program, modelMatrix);
    
    en2.setLocation(0.2f, -0.3f);
    en2.draw(program, modelMatrix);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.7f, -0.5f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Invader", 0.06f, -0.01f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.5f, -0.7f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Moves Slowly And Shoots You", 0.06f, -0.01f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Kamikaze", 0.06f, -0.01f);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.1f, -0.7f, 0.0f));
    program.SetModelMatrix(modelMatrix);
    TextureHandler::DrawText(program, font1, "Attempts To Crash Into Your Ship", 0.06f, -0.01f);
}

void GameLogic::gameUpdate(float elapsedTime, ShaderProgram& program, glm::mat4& modelMatrix) {
    particleEm.Update(elapsedTime);
    particleEm.position = glm::vec3(3.5f, 1.0f, 0.0f);
    
    if (whichScreen == TITLE || whichScreen == INSTR || whichScreen == GAMEOVER) return;
    
    if (levelRemainingEnemies == 0) {
        if (whichScreen == LVL1)
            setScreen(LVL2);
        else if(whichScreen == LVL2)
            setScreen(LVL3);
        else if(whichScreen == LVL3) {
            won = true;
            setScreen(GAMEOVER);
        }
    }

    player->update(elapsedTime);
    
    if(returnTimer >= 3.0f)
        returnTimer += elapsedTime;
    else {
        if(bullets.size() < 2) {
            std::cout << "Return fire" << std::endl;
            returnFire(elapsedTime);
            returnTimer = 0;
        }
    }
    
    enemyApproach(elapsedTime);

    clearBullets();
    for (Bullet* b: bullets)
        b->update(elapsedTime);
    collisionChecker(program, modelMatrix);
}

void GameLogic::makePlayerMove(bool dir) {
    if (dir) {
        player->move(-3.0f);
    }
    else {
        player->move(3.0f);
    }
}

void GameLogic::makePlayerShoot(float elapsed) {
    if(bullets.size() <= 2) {
        player->shoot(elapsed, bullets, pBulletTex);
        Mix_PlayChannel(-1, laser, 0);
    }
    
}

void GameLogic::returnFire(float elapsed) {
    int which = rand() % enemies.size();
    if(which >= 0 && which < enemies.size()) {
        if (enemies[which]->whichType && enemies[which]->getAlive()) {
            enemies[which]->shoot(elapsed, bullets, eBulletTex);
            Mix_PlayChannel(-1, laser, 0);
        }
    }
}

void GameLogic::enemyApproach(float elapsed) {
    if(!kamiIsComing && (whichScreen == LVL2 || whichScreen == LVL3)) {
        int whichKami = rand() % enemies.size();
        if(!enemies[whichKami]->whichType && !enemies[whichKami]->kamiState) {
            enemies[whichKami]->kamiState = true;
            kamiIsComing = true;
        }
    }
    for(auto e: enemies) {
        if((e->whichType && e->getAlive()) || (!e->whichType && !e->kamiState))
            e->setLocation(e->getXPos()+(elapsed*-0.01f), e->getYPos());
        if(!e->whichType && e->getAlive() && e->kamiState) {
            kamiIsComing = true;
            e->goForIt(player, elapsed);
        }
    }
}

void GameLogic::collisionChecker(ShaderProgram& program, glm::mat4& modelMatrix) {
    for (int i = 0; i < bullets.size(); ++i) {
        if(bullets[i]->friendly) {
            for(auto e: enemies) {
                if(bulletCollide(e, bullets[i])) {
                    if(e->getAlive()) {
                        if(!e->whichType && e->kamiState) { kamiIsComing = false; }
                        e->setAlive(false);
                        explosionEm.position = glm::vec3(e->getXPos(), e->getYPos(), 0.0f);
                        explosionEm.Render(program, modelMatrix);
                        Mix_PlayChannel(-1, enemyExplode, 0);
                        bullets[i]->xPos = 2000.0f;
                        float dX = std::abs(player->getXPos()-e->getXPos()) - ((player->getWidth()+e->getWidth())/2);
                        if (dX > 0.1) score += 5000;
                        else if (dX < 0.3) score += 1000;
                        else if (dX < 0.5) score += 500;
                        else if (dX < 1.0) score += 100;
                        else score += 50;
                        delete bullets[i];
                        bullets[i] = nullptr;
                        std::swap(bullets[i], bullets[bullets.size() - 1]);
                        bullets.pop_back();
                        i--;
                        levelRemainingEnemies--;
                        std::cout << "Enemies remaining: " << levelRemainingEnemies << std::endl;
                        break;
                    }
                }
            }
        }
        else if (player->getAlive() && !bullets[i]->friendly) {
            if (bulletCollide(player, bullets[i])) {
                if (lives > 1) {
                    lives--;
                    explosionEm.position = glm::vec3(player->getXPos(), player->getYPos(), 0.0f);
                    explosionEm.Render(program, modelMatrix);
                    Mix_PlayChannel(-1, playerExplode, 0);
                    player->setAlive(false);
                    bullets[i]->xPos = 2000.0f;
                    delete bullets[i];
                    bullets[i] = nullptr;
                    std::swap(bullets[i], bullets[bullets.size() - 1]);
                    bullets.pop_back();
                    player->setLocation(-1.6f, 0.0f);
                    player->setAlive(true);
                }
                else {
                    Mix_PlayChannel(-1, playerExplode, 0);
                    setScreen(GAMEOVER);
                    break;
                }
            }
        }
    }
    for (int i = 0; i < enemies.size(); ++i) {
        if(!enemies[i]->whichType) {
            if (kamiCollide(player, enemies[i])) {
                if (lives > 1) {
                    lives--;
                    explosionEm.position = glm::vec3(player->getXPos(), player->getYPos(), 0.0f);
                    explosionEm.Render(program, modelMatrix);
                    Mix_PlayChannel(-1, enemyExplode, 0);
                    Mix_PlayChannel(-1, playerExplode, 0);
                    player->setAlive(false);
                    enemies[i]->setAlive(false);
                    enemies[i]->setLocation(-2000.0f, -2000.0f);
                    kamiIsComing = false;
                    levelRemainingEnemies--;
                    player->setLocation(-1.6f, 0.0f);
                    player->setAlive(true);
                }
                else {
                    Mix_PlayChannel(-1, enemyExplode, 0);
                    Mix_PlayChannel(-1, playerExplode, 0);
                    setScreen(GAMEOVER);
                    break;
                }
            }
        }
    }
}

int GameLogic::currScr() { return whichScreen; }

void GameLogic::setScreen(int sc) {
    Mix_HaltMusic();
    whichScreen = sc;
    if (sc == TITLE || sc == INSTR) { Mix_PlayMusic(titleM, -1); }
    else if (sc == LVL1) { prepareLevel(LVL1); Mix_PlayMusic(gameM, -1); }
    else if (sc == LVL2) { prepareLevel(LVL2); Mix_PlayMusic(gameM, -1); }
    else if (sc == LVL3) { prepareLevel(LVL3); Mix_PlayMusic(gameM, -1); }
    else if (sc == GAMEOVER) { Mix_PlayMusic(goM, -1); }
}

void GameLogic::clearBullets() {
    for (int i = 0; i < bullets.size(); ++i) {
        if(bullets[i]->xPos > 1.777f || bullets[i]->xPos < -1.777f) {
            delete bullets[i];
            bullets[i] = nullptr;
            std::swap(bullets[i], bullets[bullets.size() -1]);
            bullets.pop_back();
            i--;
        }
    }
}

bool GameLogic::bulletCollide(Entity* poe, Bullet* wBullet) {
    float dX = std::abs(poe->getXPos()-wBullet->xPos) - ((poe->getWidth()+wBullet->width)/2);
    float dY = std::abs(poe->getYPos()-wBullet->yPos) - ((poe->getHeight()+wBullet->height)/2);
    return dX < 0 && dY < 0;
}

bool GameLogic::kamiCollide(Entity* p, Enemy* kami) {
    float dX = std::abs(p->getXPos()-kami->getXPos()) - ((p->getWidth()+kami->getWidth())/2);
    float dY = std::abs(p->getYPos()-kami->getYPos()) - ((p->getHeight()+kami->getHeight())/2);
    return dX < 0 && dY < 0;
}


void GameLogic::generalReset() {
    score = 0;
    lives = 3;
    won = false;
    bullets.clear();
    enemies.clear();
    player->setLocation(-1.6f, 0.0f);
    player->setYVel(0.0f);
    player->setYAccel(0.0f);
}
