//
//  GameState.h
//  NYUCodebase
//
//  Created by Noah Weitz on 4/2/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#ifndef GameState_h
#define GameState_h

class GameState {
private:
    int score;
    int lives;
    int screen;
    int remainingEnemies;
public:
    GameState();
    int getScreen();
    int getScore();
    int getLives();
    int getEnemies();
    void setScreen(int which);
    void subtractLife();
    void subtractEnemy();
};

#endif /* GameState_h */
