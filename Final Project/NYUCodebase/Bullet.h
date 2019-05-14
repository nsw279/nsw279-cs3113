//
//  Bullet.h
//  NYUCodebase
//
//  Created by Noah Weitz on 5/14/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#ifndef Bullet_h
#define Bullet_h

#include "Graphical.h"

class Entity;

class Bullet {
public:
    float xPos;
    float yPos;
    float xVel;
    float width;
    float height;
    float time;
    bool friendly;
    Entity* parent;
    SpriteSheet sprite;
    GLuint texture;
    
   // Bullet();
    Bullet(float x, float y, Entity* whomst, bool f, GLuint tid);
    
    void setSprite(GLuint texid, float u, float v, float w, float h, float size);
    void update(float elapsed);
    void draw(ShaderProgram& program, glm::mat4& modelMatrix);
};

#endif /* Bullet_h */
