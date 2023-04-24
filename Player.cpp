#include "Constants.h"
#include "Player.h"

Player::Player() {
    dx = dy = 0;
    currentFrame = 0;
    score = 0.0;
    tripin = false;
}

Player::Player(Texture &image, Texture &h) {
    sprite.setTexture(image);
    sprite.setTextureRect(IntRect(82, 144, 14, 16));

    heart.setTexture(h);
    heart.setTextureRect(IntRect(121, 140, 218, 319));
    heart.setScale(0.15, 0.15);

    rect = FloatRect(4 * 16, (H - 4) * 16, 14, 16);

    currentFrame = 0.0;
    dx = dy = 0;
    score = 0;
    tripin = false;
}

void Player::update(float time, std::string *TileMap) {
    rect.left += dx * time;
    Collision(0, TileMap);

    if (!onGround) {
        dy += 0.0005 * time;
        if (dx > 0) {
            sprite.setTextureRect(IntRect(230, 145, 16, 16));
        }

        if (dx < 0) {
            sprite.setTextureRect(IntRect(246, 145, -16, 16));
        }
    }
    rect.top += dy * time;
    onGround = false;
    Collision(1, TileMap);

    if (currentFrame > 5) {
        currentFrame -= 5;
    }

    if (dx > 0 && onGround) {
        currentFrame += 0.025 * time;
        sprite.setTextureRect(IntRect(30 * int(currentFrame) + 82, 144, 14, 16));
    }

    if (dx < 0 && onGround) {
        currentFrame += 0.025 * time;
        sprite.setTextureRect(IntRect(30 * int(currentFrame) + 95, 144, -14, 16));
    }

    if (rect.top > 488) {
        Life--;
        dy = -0.35;
    }

    sprite.setPosition(rect.left - offsetX, rect.top);

    dx = 0;
}

void Player::animation(float time) {
    if (!onGround) {
        rect.top += 0.065 * time;
    } else {
        rect.left += 0.07 * time;
    }

    if (rect.top > (H - 4) * 16) {
        onGround = true;
        rect.top = (H - 4) * 16;
    }

    if (currentFrame > 5) {
        currentFrame -= 5;
    }

    currentFrame += 0.015 * time;

    if (onGround) {
        sprite.setTextureRect(IntRect(30 * int(currentFrame) + 82, 144, 14, 16));
    } else {
        sprite.setTextureRect(IntRect(230, 145, 16, 16));
    }

    sprite.setPosition(rect.left - offsetX, rect.top);
}

void Player::Collision(int num, std::string *TileMap) {
    for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) {
        for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
            if (TileMap[i][j] == '0' || TileMap[i][j] == 'R' || TileMap[i][j] == 'k'
            || TileMap[i][j] == 'e' || TileMap[i][j] == 'b') {
                if (dx > 0 && num == 0) {
                    rect.left = j * 16 - rect.width;
                }

                if (dx < 0 && num == 0) {
                    rect.left = j * 16 + 16;
                }

                if (dy > 0 && num == 1) {
                    rect.top = i * 16 - rect.height;
                    dy = 0;
                    onGround = true;
                }

                if (dy < 0 && num == 1) {
                    rect.top = i * 16 + 16;
                    dy = 0;
                }
            }
        }
    }
}

void Player::draw_lives(RenderWindow *window) {
    for (int i = 0; i < int(Life); i++) {
        heart.setPosition(i * 35 + 805, 10);
        window->draw(heart);
    }
}

void Player::show_score(RenderWindow *window, Text &text) {
    std::ostringstream cc;
    cc << "score x " << int(score);
    text.setString(cc.str());
    window->draw(text);
}
