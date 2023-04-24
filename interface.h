#pragma once
#ifndef HELLOSFML_INTERFACE_H
#define HELLOSFML_INTERFACE_H

#include "Constants.h"

class Button {
protected:              // protected поля дают доступ к ним только кнутри самого класса и производных классов
    Sprite sprite;
    Color color;
    void (*f)();

public:
    Button(Texture &image);

    void set_scale(float x, float y);

    void set_texture_rect(int x, int y, int width, int height);

    void update(RenderWindow* window);

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    void set_position(int x, int y);

    void set_trigger(void (*func)()) {
        f = func;
    }
};

// кнопка, позволяющая реализовывать себя с помощью текста
class TextButton {
private:
    void (*f)(void);
    Text text;
    Color color;
public:
    TextButton(Font &font);

    void set_position(float x, float y);

    void set_color(Color color);

    void set_string(std::string &new_str);

    void set_trigger(void (*func)(void));

    void update(RenderWindow* window);

    void draw(RenderWindow* window);
};

// суперкласс меню
class Menu {
protected:
    Sprite backGround;
    std::vector<Button> buttons;
public:
    Menu() { }

    void add_button(Button &button);

    void setBackground(Texture &image, float x, float y);

    void clear();
};

class AnimatedMenu : public Menu {
private:
    Sprite move;
public:
    AnimatedMenu() { }

    AnimatedMenu(Texture &animated);

    void set_move_rect(Vector2<int> point, Vector2<int> size);

    void set_move_position(float x, float y);

    void display(float time, RenderWindow* window);
};

class TextMenu {
protected:
    std::vector<TextButton> buttons;
public:
    TextMenu();

    void add_button(TextButton &button);

    void display(RenderWindow* window);

    void clear();
};


#endif //HELLOSFML_INTERFACE_H