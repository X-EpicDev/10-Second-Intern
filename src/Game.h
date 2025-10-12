#ifndef INC_10SECONDINTERN_GAME_H
#define INC_10SECONDINTERN_GAME_H

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Object.h"
#include "Player.h"
#include "raylib.h"
#include "Task.h"

enum GameState {
    WAITING,
    PLAYING,
    FINISHED
};

class Game {
private:
    int windowWidth = 1080;
    int windowHeight = 720;
    float movementSpeed = 75.0f;
    bool debug = false;

    GameState gameState = WAITING;
    float timer = 10;
    float cooldown = 2.5f;
    int score = 0;
    Task* currentTask = nullptr;

    std::vector<Task> tasks;
    std::unordered_map<Types, std::vector<Object>> objects;
public:
    void start();
};


#endif