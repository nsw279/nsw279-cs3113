//
//  Entity.h
//  NYUCodebase
//
//  Created by Noah Weitz on 4/16/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

#define TILE_SIZE 0.7f

#include "Graphical.h"

class Entity {
protected:
    float height;
    float width;
    float xPos;
    float yPos;
    bool isStatic;
    bool alive;
    SpriteSheet sprite;
private:
    float xVel;
    float yVel;
    float xAccel;
    float yAccel;
    float gravity;
    float friction;
    
    bool upCollide;
    bool downCollide;
    bool leftCollide;
    bool rightCollide;
    
public:
    Entity(); // Constructors
    Entity(float h, float w, float xp, float yp, float xv, float yv, float xa, float ya, float grav, bool sOrD);
    void setSprite(GLuint texid, float u, float v, float w, float h, float size);
    
    // Getter methods
    float getHeight();
    float getWidth();
    float getXPos();
    float getYPos();
    float getXVel();
    float getYVel();
    float getXAccel();
    float getYAccel();
    float getGravity();
    float lerp(float v0, float v1, float t);
    
    bool staticTrueDynamicFalse();
    
    // Other Methods
    void setLocation(float newX, float newY);
    void move(float acc);
    void madeCollision(char dir);
    void jump();
    
    // Stuff important to the game
    void update(float elapsedTime);
    void draw(ShaderProgram& program, glm::mat4& modelMatrix);
    SpriteSheet* getSprite();
};

class Ground : public Entity {
public:
    Ground();
    Ground(float xp, float yp, float sz);
    bool hit;
    void beHit();
};

#endif /* Entity_h */
