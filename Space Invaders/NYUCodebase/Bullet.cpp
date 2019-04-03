//
//  Bullet.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 4/2/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include "Bullet.h"

Bullet::Bullet(bool allegiance) : player(allegiance), width(0.1), height(0.3) {}

bool Bullet::isPlayers() { return player; }

bool Bullet::checkHitBoundary() {
    return (yPos > 10 || yPos < -10);
}

bool Bullet::colliding(float otherX, float otherY, float otherW, float otherH) {
    float px = abs(xPos - otherX) - ((width + otherW) / 2);
    float py = abs(yPos - otherY) - ((height + otherW) / 2);
    return px < 0 && py < 0 && active;
}

void Bullet::beShot(bool player, float originX, float originY, float originW, float originH) {
    xPos = originX;
    yPos = originY;
    if(player) {
        while (active) {
            yPos += 0.1f;
            if (checkHitBoundary() || colliding(originX, yPos, originW, originH)) { active = false; }
        }
    }
    else {
        while (active) {
             yPos += -0.1f;
            if (!checkHitBoundary() || colliding(originX, yPos, originW, originH)) {active = false; }
        }
    beDestroyed();
    }
}

void Bullet::beDestroyed() { xPos = -2000.0f; yPos = -2000.0f; }

float Bullet::getX() { return xPos; }
float Bullet::getY() { return yPos; }
float Bullet::getW() { return width; }
float Bullet::getH() { return height; }
