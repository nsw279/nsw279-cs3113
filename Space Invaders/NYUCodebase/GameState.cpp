//
//  GameState.cpp
//  NYUCodebase
//
//  Created by Noah Weitz on 4/2/19.
//  Copyright © 2019 Ivan Safrin. All rights reserved.
//

#include <stdio.h>

#include "GameState.h"

GameState::GameState() : score(0), lives(3), screen(0), remainingEnemies(8) {}

int GameState::getScreen() { return screen; }
int GameState::getScore() { return score; }
int GameState::getLives() { return lives; }
int GameState::getEnemies() { return remainingEnemies; }

void GameState::setScreen(int which) { screen = which; }

void GameState::subtractLife() { lives--; }

void GameState::subtractEnemy() { remainingEnemies--; }
