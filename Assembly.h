#pragma once
#ifndef HELLOSFML_ASSEMBLY_H
#define HELLOSFML_ASSEMBLY_H

#include "Constants.h"
#include "Player.h"
#include <vector>

class Coin {
public:
    FloatRect rect;          // прямоугольник, с которым мы будем считать пересечение модели игрока
    float currentFrame = 0;  // текущий кадр анимации
    Sprite s;
    // базовый конструктор
    Coin();
    // конструктор со ссылкой на текстуру объекта
    Coin(Texture &image);
    void update(float &time);
    // функция для задания позиции монетки на экране
    void set(float x, float y);

    bool is_in_view();
};

class MushRoom : public Coin {
public:
    bool scored;
    Clock active;
    int type;
    Text time;

    MushRoom();

    MushRoom(Texture &image, int _type_, Font &font);

    void activate(Player &player);

    int get_time();

    void draw_time(RenderWindow* window);

    void update();
};

class Bonus {
public:
    FloatRect rect;
    Sprite s;
    bool scored;
    float max_cordY;

    Bonus(Texture &image);

    void set(float x, float y);

    void update(float &time);
    // замняем пустое пространство на карте заполненным блоком
    void delete_from_map(std::string *TileMap);
    // если бонус собран, то мы замeняем его на монетку
    void emplace_with_coin(std::vector<Coin *> &coins, Texture &image);

    void emplace_with_mushroom(std::vector<MushRoom *> &mushRooms, Texture &image, Font &font);

    bool is_in_view();
};

#endif //HELLOSFML_ASSEMBLY_H
