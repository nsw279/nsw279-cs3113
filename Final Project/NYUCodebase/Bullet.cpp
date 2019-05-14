//
//  Bullet.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 5/14/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include "Bullet.h"
#include "Entity.h"

//Bullet::Bullet() {}

Bullet::Bullet(float x, float y, Entity* whomst, bool f, GLuint tid) : xPos(x), yPos(y), parent(whomst), friendly(f), texture(tid) {
    height = 0.2f;
    width = 0.1f;
    xVel = 2.0f;
    this->setSprite(texture, 1.0f, 1.0f, width, height, 0.1f);
}

void Bullet::setSprite(GLuint texid, float u, float v, float w, float h, float size) { this->sprite = SpriteSheet(size, texid, u, v, w, h); }

void Bullet::update(float elapsed){
    if(friendly) {
        if(xPos < 1.777f)
            xPos += elapsed * xVel;
    }
    else {
        if(xPos > -1.777f)
            xPos -= elapsed * xVel;
    }
}

void Bullet::draw(ShaderProgram& program, glm::mat4& modelMatrix) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(width, height, 1.0f));
        
        sprite.Draw(program, modelMatrix, sprite.getTID(), xPos, yPos);
}
