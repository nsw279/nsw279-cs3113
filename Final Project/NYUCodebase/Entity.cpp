//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 4/16/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include "Entity.h"
#include "Bullet.h"

Entity::Entity() {}

Entity::Entity(float h, float w, float xp, float yp, float xv, float yv, float xa, float ya, float grav) :
 height(h), width(w), xPos(xp), yPos(yp), xVel(xv), yVel(yv), xAccel(xa), yAccel(ya), gravity(grav), friction(10.0f) {
     if(xPos == -1.6) isPlayer = true;
 }

float Entity::getHeight() { return height; }
float Entity::getWidth() { return width; }
float Entity::getXPos() { return xPos; }
float Entity::getYPos() { return yPos; }
float Entity::getXVel() { return xVel; }
float Entity::getYVel() { return yVel; }
float Entity::getXAccel() { return xAccel; }
float Entity::getYAccel() { return yAccel; }
float Entity::getGravity() { return gravity; }
float Entity::lerp(float v0, float v1, float t) { return (1.0f-t)*v0 + t*v1; }

void Entity::setLocation(float newX, float newY) { xPos = newX; yPos = newY; }
void Entity::move(float acc) {
    if (yPos < 0.85f && yPos > -0.9f)
        yAccel = acc;
}

void Entity::setPlayer() { isPlayer = true; }
void Entity::setEnemy() { isPlayer = false; }

void Entity::shoot(float elapsed, std::vector<Bullet*>& bullets, GLuint bTex) {
    bullets.push_back(new Bullet(this->getXPos(), this->getYPos(), this, isPlayer, bTex));
}

void Entity::setSprite(GLuint texid, float u, float v, float w, float h, float size) { this->sprite = SpriteSheet(size, texid, u, v, w, h); }



void Entity::update(float elapsedTime) {
    if(yPos > -0.9f && yPos < 0.85f)
        this->setLocation(xPos, yPos+(elapsedTime*yVel));
    else if (yPos < -0.9f) {
        this->setLocation(xPos, -0.89f);
        this->yAccel = 0;
    }
    else if (yPos > 0.85f) {
        this->setLocation(xPos, 0.84f);
        this->yAccel = 0;
    }
    
    if(yAccel > 0.0f) {yAccel -= friction * elapsedTime;}
    else if(yAccel < 0.0f) { yAccel += friction * elapsedTime;}
    
    yPos += yAccel * elapsedTime;
    
    if (yVel > 0.0f) {
        if(yVel-= friction*elapsedTime != 0)
            yVel = 0.0f;
        else
            yVel -= friction * elapsedTime;
    }
    else if (yVel < 0.0f) {
        if(yVel += friction * elapsedTime != 0)
            yVel = 0.0f;
        else
            yVel += friction * elapsedTime;
        
    }
}

void Entity::draw(ShaderProgram& program, glm::mat4& modelMatrix) {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(width, height, 1.0f));

    sprite.Draw(program, modelMatrix, sprite.getTID(), xPos, yPos);
}

SpriteSheet* Entity::getSprite() { return &sprite; }

bool Entity::getAlive() { return alive; }

bool Entity::getActive() { return active; }

void Entity::setAlive(bool val) { alive = val; }

void Entity::setYVel(float val) { yVel = val; }

void Entity::setYAccel(float val) { yAccel = val; }

Enemy::Enemy() {}

Enemy::Enemy(bool type) : whichType(type) {
    height = 0.4f;
    width = 0.4f;
    alive = true;
    kamiState = false;
}

void Enemy::goForIt(Entity* target, float elapsed) {
    if(this->getXPos() > -1.777f)
        this->setLocation(this->getXPos()+(elapsed*-1.1f), this->getYPos());
    else
        this->setLocation(1.69f, this->getYPos());
    if(this->yPos < target->getYPos()) { this->yPos += elapsed * 0.5f; }
    else { this->yPos -= elapsed * 0.5f; }
}

