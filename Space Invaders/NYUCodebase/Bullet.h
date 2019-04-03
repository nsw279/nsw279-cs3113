//
//  Bullet.h
//  NYUCodebase
//
//  Created by Noah Weitz on 4/2/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#ifndef Bullet_h
#define Bullet_h

#include <cmath>

class Bullet {
private:
    bool player;
    bool active;
    float xPos;
    float yPos;
    float width;
    float height;
public:
    Bullet(bool allegiance);
    bool isPlayers();
    bool checkHitBoundary();
    bool colliding(float otherX, float otherY, float otherW, float otherH);
    void beShot(bool player, float originX, float originY, float originW, float originH);
    void beDestroyed();
    float getX();
    float getY();
    float getW();
    float getH();
};

#endif /* Bullet_h */
