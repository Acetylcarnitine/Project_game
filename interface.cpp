#include "interface.h"

Button::Button(Texture &image) {
    sprite.setTexture(image);
}

void Button::set_texture_rect(int x, int y, int width, int height) {
    sprite.setTextureRect(IntRect(x, y, width, height));
}

void Button::set_scale(float x, float y) {
    sprite.setScale(x, y);
}

void Button::set_position(int x, int y) {
    sprite.setPosition(x, y);
}

void Button::update(RenderWindow* window) {
    if (sprite.getGlobalBounds().contains(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y)) {
        sprite.setColor(Color::Cyan);
        if (Mouse::isButtonPressed(Mouse::Button::Left)) {
            f();
        }
    }
}

TextButton::TextButton(Font &font) {
    text = Text("N", font, 30);
}

void TextButton::set_string(std::string &new_str) {
    text.setString(new_str);
}

void TextButton::set_position(float x, float y) {
    text.setPosition(x, y);
}

void TextButton::set_color(Color _color_) {
    color = _color_;
}

void TextButton::set_trigger(void (*func)()) {
    f = func;
}

void TextButton::update(RenderWindow* window) {
    if (text.getGlobalBounds().contains(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y)) {
        text.setFillColor(color);
        if (Mouse::isButtonPressed(Mouse::Button::Left)) {
            f();
        }
    } else {
        text.setFillColor(Color::White);
    }
}

void TextButton::draw(RenderWindow* window) {
    window->draw(text);
}

void Menu::add_button(Button &button) {
    buttons.push_back(button);
}

void Menu::setBackground(Texture &image, float x, float y) {
    backGround.setTexture(image);
    backGround.setScale(x, y);
}

void Menu::clear() {
    buttons.clear();
}

AnimatedMenu::AnimatedMenu(Texture &animated) {
    move.setTexture(animated);
}

void AnimatedMenu::set_move_rect(Vector2<int> point, Vector2<int> size) {
    move.setTextureRect(IntRect(point.x, point.y, size.x, size.y));
}

void AnimatedMenu::set_move_position(float x, float y) {
    move.setPosition(x, y);
}

void AnimatedMenu::display(float time, RenderWindow* window) {
    move.move(0.075 * time, 0);

    if (move.getPosition().x > 912) {
        move.setPosition(-620, 90);
    }

    for (auto i : buttons) {
        i.update(window);
    }

    window->draw(backGround);
    window->draw(move);

    for (auto i : buttons) {
        i.draw(*window);
    }
}

TextMenu::TextMenu() { }

void TextMenu::add_button(TextButton &button) {
    buttons.push_back(button);
}

void TextMenu::display(RenderWindow *window) {
    for (auto &i : buttons) {
        i.update(window);
    }

    for (auto &i : buttons) {
        i.draw(window);
    }
}

void TextMenu::clear() {
    buttons.clear();
}