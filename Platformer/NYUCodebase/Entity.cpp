//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 4/16/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

Entity::Entity(){}

Entity::Entity(float h, float w, float xp, float yp, float xv, float yv, float xa, float ya, float grav, bool sOrD) :
 height(h), width(w), xPos(xp), yPos(yp), xVel(xv), yVel(yv), xAccel(xa), yAccel(ya), gravity(grav), friction(10.0f), isStatic(sOrD) {}

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

bool Entity::staticTrueDynamicFalse() { return isStatic; }

void Entity::setLocation(float newX, float newY) { xPos = newX; yPos = newY; }
void Entity::move(float acc) { xAccel = acc; }
void Entity::jump() { if(downCollide) {yVel = 5.0f;} }

void Entity::setSprite(GLuint texid, float u, float v, float w, float h, float size) { this->sprite = SpriteSheet(size, texid, u, v, w, h); }

void Entity::madeCollision(char dir) {
    if (dir == 'u') { upCollide = true; }
    else if (dir == 'd') { downCollide = true; }
    else if (dir == 'l') { leftCollide = true; }
    else if (dir == 'r') { rightCollide = true; }
}

void Entity::update(float elapsedTime) {
    std::cout << "Down collide: " << downCollide << ", yPos: " << yPos <<std::endl;
    upCollide = false; downCollide = false; leftCollide = false; rightCollide = false;
    
    this->setLocation(xPos+(elapsedTime*xVel), yPos+(elapsedTime*yVel));
    
    if(xAccel > 0.0f) {xAccel -= friction * elapsedTime;}
    else if(xAccel < 0.0f) { xAccel += friction * elapsedTime;}
    
    xVel += xAccel * elapsedTime;
    yVel += (yAccel - gravity) * elapsedTime;
    
    if (xVel > 0.0f) { xVel -= friction * elapsedTime; }
    else if (xVel < 0.0f) { xVel += friction * elapsedTime; }
}

void Entity::draw(ShaderProgram& program, glm::mat4& modelMatrix) {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(xPos, yPos, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(width, height, 1.0f));

    sprite.Draw(program, modelMatrix, sprite.getTID(), xPos, yPos);
}

SpriteSheet* Entity::getSprite() { return &sprite; }

Ground::Ground() {}

Ground::Ground(float xp, float yp, float sz) {
    height = sz;
    width = sz;
    xPos = xp;
    yPos = yp;
    isStatic = true;
    hit = false;
}

void Ground::beHit() {
    hit = true;
}

