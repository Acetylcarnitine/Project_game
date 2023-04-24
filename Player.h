#pragma once
#ifndef HELLOSFML_PLAYER_H
#define HELLOSFML_PLAYER_H

#include "Constants.h"

using namespace sf;

class Player {
public:
    Sprite sprite;
    FloatRect rect;         // прямоугольник игрока
    bool onGround;          // переменная для реализации гравитации
    bool tripin;
    float dx, dy;           // скорость игрока по осям
    float currentFrame;     // текущий кадр анимации
    float Life = 3;         // количество жизней
    float score;            // счет игрока
    Sprite heart;

    Player();

    Player(Texture &image, Texture &h);

    void update(float time, std::string *TileMap);
    // анимация по завершению уровня
    void animation(float time);
    // проверка игрока на столкновения его с другими объектами
    void Collision(int num, std::string *TileMap);
    // отображние жизней игрока
    void draw_lives(RenderWindow* window);
    // отрисовка счета на экране
    void show_score(RenderWindow* window, Text &text);
};

#endif //HELLOSFML_PLAYER_H