#pragma once
#ifndef HELLOSFML_ENEMY_H
#define HELLOSFML_ENEMY_H

#include "Player.h"

using namespace sf;

class Enemy {
public:
    bool alive;                     // жив противник или нет
    float dx;                       // скорость по оси ОХ
    float currentFrame;             // текущий кадр анимации
    std::pair<int, int> walls;      // область, в которой противник "видит" игрока
    FloatRect rect;                 // модель противника
    float disappear;                // переменная для отслеживания удалять противника или нет
    Sprite sprite;

    bool is_in_view() {
        if (walls.first * 16 - offsetX < 912 && walls.second * 16 - offsetX > 0) {
            return true;
        } else {
            return false;
        }
    }

    virtual void set(Texture &image, int x, int y, std::string *TileMap) {}

    virtual void update(float time, Player &p, std::string *TileMap) {}
};

class Goomba : public Enemy {
public:
    Goomba();

    void set(Texture &image, int x, int y, std::string *TileMap) override;

    void update(float time, Player &p, std::string *TileMap) override;

    void Collision(std::string *TileMap);

    std::pair<int, int> find_walls(int x, std::string *TileMap);


};

class KoopaTroopa : public Enemy {
public:
    float dy;           // добавляем скорость движения по оси OY
    bool onGround;      // индикатор находится ли противник на земле или нет

    KoopaTroopa();

    void set(Texture &image, int x, int y, std::string *TileMap) override;

    void jump();

    void update(float time, Player &player, std::string *TileMap) override;

    void Collision(int num, std::string *TileMap);

    void find_obstacle(std::string *TileMap);

    std::pair<int, int> find_walls(int x, std::string *TileMap);
};

#endif //HELLOSFML_ENEMY_H
