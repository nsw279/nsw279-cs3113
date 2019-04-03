//
//  Entity.h
//  NYUCodebase
//
//  Created by Noah Weitz on 4/2/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

#include "ShaderProgram.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SpriteSheet.h"
#include "Bullet.h"

class Entity {
private:
    float height;
    float width;
    float xPos;
    float yPos;
    bool alive;
    bool type;
    GLuint graphic;
public:
    Entity(bool allegiance, GLuint graphic);
    
    float getHeight();
    float getWidth();
    float getX();
    float getY();
    
    bool isAlive();
    bool getType();
    bool colliding(float otherX, float otherY, float otherW, float otherH);
    
    void moveX(float d);
    void moveY(float d);
    
    void destroy();
    void shootBullet(bool player, Bullet b);
    
    void draw(ShaderProgram& program, glm::mat4& modelMatrix);
};

#endif /* Entity_h */
