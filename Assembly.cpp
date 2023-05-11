#include "Assembly.h"
#include <iostream>

Coin::Coin() {
    rect = FloatRect(0, 0, 19, 19);
    currentFrame = 0;
}

Coin::Coin(Texture &image) {
    currentFrame = 0;
    s.setTextureRect(IntRect(0, 0, 63, 63));
    s.setTexture(image);
    s.setScale(0.3, 0.3);
    rect = FloatRect(300, 300, 19, 19);
}

void Coin::set(float x, float y) {
    rect.left = x;
    rect.top = y;
    s.setPosition(rect.left, rect.top);
}

void Coin::update(float &time) {
    currentFrame += 0.005 * time;

    if (currentFrame > 8) {
        currentFrame -= 8;
    }

    s.setTextureRect(IntRect(63 * (int) currentFrame, 0, 63, 63));

    s.setPosition(rect.left - offsetX, rect.top);
}

bool Coin::is_in_view() {
    if (rect.left <= offsetX + 700 && rect.left + rect.width >= offsetX) {
        return true;
    } else {
        return false;
    }
}

Bonus::Bonus(Texture &image) {
    rect = {0, 0, 16, 16};
    scored = false;
    s.setTexture(image);
    s.setTextureRect({128, 112, 16, 16});
}

void Bonus::set(float x, float y) {
    rect.left = x;
    rect.top = y;
    s.setPosition(rect.left - offsetX, rect.top);
    max_cordY = rect.top - 20;
}

void Bonus::update(float &time) {
    if (scored) {
        rect.top -= 0.1 * time;
        s.setPosition(rect.left - offsetX, rect.top);
    } else {
        s.setPosition(rect.left - offsetX, rect.top);
    }
}

void Bonus::delete_from_map(std::string *TileMap) {
    int x = rect.left / 16;
    int y = (max_cordY + 20) / 16;
    TileMap[y][x] = 'e';
}

void Bonus::emplace_with_coin(std::vector<Coin *> &coins, Texture &image) {
    Coin *coin = new Coin(image);
    coin->set(rect.left, rect.top);
    coins.push_back(coin);
}

void Bonus::emplace_with_mushroom(std::vector<MushRoom *> &mushRooms, Texture &image, Font &font) {
    MushRoom *mush = new MushRoom(image, font);
    mush->set(rect.left, rect.top);
    mushRooms.push_back(mush);
}

bool Bonus::is_in_view() {
    if (rect.left <= offsetX + 700 && rect.left + rect.width >= offsetX) {
        return true;
    } else {
        return false;
    }
}

MushRoom::MushRoom() {
    rect = {0, 0, 15, 16};
    scored = false;
}

MushRoom::MushRoom(Texture &image, int _type_, Font &font) : Coin(image) {
    rect = {0, 0, 15, 16};
    s.setTextureRect({0, 0, 15, 16});
    s.setScale(1, 1);
    scored = false;
    type = _type_;
    time = Text("N", font, 25);
    time.setPosition(10, 40);
}

void MushRoom::activate(Player &player) {
    if (!player.tripin) {
        active.restart();
        scored = true;
    }
}

int MushRoom::get_time() {
    return active.getElapsedTime().asMilliseconds();
}

void MushRoom::draw_time(RenderWindow* window) {
    time.setString(std::to_string(float(double(get_time())/ 1000)));
    if (get_time() > 3000) {
        time.setFillColor(Color::Red);
    }
    window->draw(time);
}

void MushRoom::update() {
    s.setPosition(rect.left - offsetX, rect.top);
}
