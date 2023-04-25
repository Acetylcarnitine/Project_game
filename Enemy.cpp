#include "Enemy.h"

template<typename T>
int sign_of(T variable) {
    if (variable > 0) {
        return 1;
    } else if (variable < 0) {
        return -1;
    } else {
        return 0;
    }
}

int random_from(int left_range, int right_range) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(left_range, right_range);
    return dist(rng);
}

Goomba::Goomba() {
    alive = false;
    dx = 0;
    disappear = 0;
}

void Goomba::set(Texture &image, int x, int y, std::string *TileMap) {
    sprite.setTexture(image);
    sprite.setTextureRect(IntRect(0, 0, 16, 16));
    rect = FloatRect(x, y, 16, 16);
    walls = find_walls(x, TileMap);
    alive = true;
    if (random_from(1, 2) == 1) {
        dx = -0.06;
    } else {
        dx = 0.06;
    }
}

void Goomba::update(float time, Player &p, std::string *TileMap) {
    if (alive && is_in_view()) {
        rect.left += dx * time;

        if (p.rect.left > walls.first * 16 && p.rect.left < walls.second * 16 && p.rect.top <= rect.top) {
            if (p.rect.top / 16 == H - 4) {
                dx = sign_of(p.rect.left - rect.left) * 0.075;
            }
        } else {
            dx = sign_of(dx) * 0.05;
        }

        Collision(TileMap);

        currentFrame += 0.005 * time;

        if (currentFrame > 2) {
            currentFrame -= 2;
        }

        sprite.setTextureRect(IntRect(18 * int(currentFrame), 0, 16, 16));
    } else {
        if (disappear < 6 && !alive) {
            disappear += 0.0005 * time;
            sprite.setTextureRect(IntRect(58, 0, 16, 16));
        }
    }

    sprite.setPosition(rect.left - offsetX, rect.top);
}

void Goomba::Collision(std::string *TileMap) {
    for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
        if (TileMap[int(rect.top / 16)][j] == 'k' || TileMap[int(rect.top / 16)][j] == 'e' ||
            TileMap[int(rect.top / 16)][j] == 'a') {
            dx *= -1;
        }
    }
}

std::pair<int, int> Goomba::find_walls(int x, std::string *TileMap) {
    x /= 16;
    int y = x;

    while (TileMap[H - 4][x] != 'k' && TileMap[H - 4][x] != 'a') {
        x--;
    }

    while (TileMap[H - 4][y] != 'k' && TileMap[H - 4][y] != 'a') {
        y++;
    }

    return std::pair<int, int>{x, y};
}

KoopaTroopa::KoopaTroopa() {
    alive = false;
    onGround = false;
    dx = dy = 0;
    disappear = 0;
}

void KoopaTroopa::set(Texture &image, int x, int y, std::string *TileMap) {
    sprite.setTexture(image);
    sprite.setTextureRect(IntRect(96, 11, 16, 18));
    alive = true;
    onGround = true;
    dx = -0.065;
    rect = FloatRect(x - 4, y - 4, 16, 20);
    walls = find_walls(x, TileMap);
}

void KoopaTroopa::jump() {
    if (onGround) {
        onGround = false;
        dy = -0.325;
    }
}

void KoopaTroopa::update(float time, Player &player, std::string *TileMap) {
    if (alive && is_in_view()) {
        rect.left += dx * time;
        Collision(0, TileMap);

        if (!onGround) {
            dy += 0.0005 * time;
        }
        rect.top += dy * time;
        onGround = false;
        Collision(1, TileMap);

        currentFrame += 0.005 * time;
        if (currentFrame > 2) {
            currentFrame -= 2;
        }

        if (player.rect.left > walls.first * 16 && player.rect.left < walls.second * 16 && onGround &&
            player.rect.top >= rect.top) {
            dx = sign_of(player.rect.left - rect.left) * 0.075;
        } else {
            dx = sign_of(dx) * 0.065;
        }

        if (dx > 0) {
            sprite.setTextureRect(IntRect(112 + int(currentFrame) * 16, 11, -16, 20));
        }

        if (dx < 0) {
            sprite.setTextureRect(IntRect(96 + int(currentFrame) * 16, 11, 16, 20));
        }

        find_obstacle(TileMap);
    }

    if (!onGround && !alive) {
        sprite.setTextureRect(IntRect(161, 10, 16, 20));
        dy += 0.05 * time;
        rect.top += dy * time;
        onGround = false;
        Collision(1, TileMap);
    }
    if (!alive) {
        sprite.setTextureRect(IntRect(161, 10, 16, 20));

        if (disappear < 5) {
            disappear += 0.0005 * time;
        }

        dx = 0;
        dy = 0;
    }

    sprite.setPosition(rect.left - offsetX, rect.top);
}

void KoopaTroopa::Collision(int num, std::string *TileMap) {
    for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) {
        for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
            if (TileMap[i][j] == '0' || TileMap[i][j] == 'R' || TileMap[i][j] == 'k' || TileMap[i][j] == 'e') {
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

void KoopaTroopa::find_obstacle(std::string *TileMap) {
    if (TileMap[int(rect.top / 16)][int(rect.left / 16)] == 'a') {
        dx *= -1;
    }

    if (dx > 0) {
        int currentTile = int(rect.left / 16);
        for (int j = currentTile; j < currentTile + 3; j++) {
            if (TileMap[int(rect.top / 16)][j] == 'k') {
                jump();
                break;
            }

            if (TileMap[H - 3][j] == ' ' && std::abs(currentTile - j) == 1) {
                jump();
                break;
            }
        }
    } else {
        int currentTile = int(rect.left / 16);
        for (int j = currentTile; j > currentTile - 3; j--) {
            if (TileMap[int(rect.top / 16)][j] == 'k') {
                jump();
                break;
            }

            if (TileMap[H - 3][j] == ' ' && currentTile - j == 0) {
                jump();
                break;
            }
        }
    }
}

std::pair<int, int> KoopaTroopa::find_walls(int x, std::string *TileMap) {
    x /= 16;
    int y = x;
    while (TileMap[int(rect.top / 16)][x] != 'a') {
        x--;
    }
    while (TileMap[int(rect.top / 16)][y] != 'a') {
        y++;
    }
    return std::make_pair(x, y);
}