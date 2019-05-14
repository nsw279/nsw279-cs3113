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

#define TITLE 0
#define LVL1 1
#define LVL2 2
#define LVL3 3
#define GAMEOVER 4
#define INSTR 5

#include "Graphical.h"
#include "Entity.h"
#include "Bullet.h"

class GameLogic {
private:
    int score;
    int lives;
    int whichScreen;
    float levelRemainingEnemies;

    float returnTimer;
    
    bool won;
    bool kamiIsComing;
    
    Entity* player;
    GLuint font1;
    GLuint playerTexture;
    GLuint enemyTex1;
    GLuint enemyTex2;
    GLuint barrierTex;
    GLuint pBulletTex;
    GLuint eBulletTex;
    Mix_Chunk *playerExplode;
    Mix_Chunk *enemyExplode;
    Mix_Chunk *laser;
    Mix_Music *titleM;
    Mix_Music *gameM;
    Mix_Music *goM;
    ParticleEmitter particleEm;
    ExplosionEmitter explosionEm;
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;
public:
    GameLogic();
    
    void gameDraw(ShaderProgram& program, glm::mat4& modelMatrix);
    void mapDraw(ShaderProgram& program, glm::mat4& modelMatrix, int which);
    void drawTitle(ShaderProgram& program, glm::mat4& modelMatrix);
    void drawLevel(ShaderProgram& program, glm::mat4& modelMatrix, int which);
    void drawGO(ShaderProgram& program, glm::mat4& modelMatrix);
    void drawInstr(ShaderProgram& program, glm::mat4& modelMatrix);
    void gameUpdate(float elapsedTime, ShaderProgram& program, glm::mat4& modelMatrix);
    void collisionChecker(ShaderProgram& program, glm::mat4& modelMatrix);
    void makePlayerMove(bool dir);
    void makePlayerShoot(float elapsed);
    void returnFire(float elapsed);
    void enemyApproach(float elapsed);
    void setScreen(int sc);
    void clearBullets();
    void prepareLevel(int which);
    void generalReset();
    
    bool bulletCollide(Entity* poe, Bullet* wBullet);
    bool kamiCollide(Entity* p, Enemy* kami);
    
    int currScr();

};

#endif /* GameLogic_h */
