#include "Constants.h"
#include "interface.h"
#include "PLayer.h"
#include "Enemy.h"
#include "Assembly.h"

// offsetX используется для смещения карты в зависимости от положения игрока
float offsetX = 0;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"

// вспомогательный класс для управления игрой
// переключениями между меню и самой игрой, смена уровней и их загрузка
class Game {
public:
    AnimatedMenu MainMenu;
    TextMenu PauseMenu, FinishMenu;

    // переменные для переключения между меню и уровнями
    bool start;
    bool level_finished;
    int level;
    int menuNum;

    // все текстуры игры
    Texture tile, heart, move,
            backGround, con, flag,
            buttons, koopa, items;
    Sprite plate, fl;

    Music music;
    SoundBuffer buffer;
    Sound jump;

    // поля для создания текстового меню и экрана смерти при поражении
    Font font;
    Text text, death_str;

    // поля внутриигровых персонажей
    Player player;
    std::vector<Enemy *> enemies;
    std::vector<Coin *> coins;
    std::vector<Bonus *> bonuses;
    std::vector<MushRoom *> mushRooms;

    //карта тайлов 16 * 16
    std::string TileMap[H];

    Game() {
        level = 1;

        buttons.loadFromFile("images/buttons.png");
        backGround.loadFromFile("images/emptyground.png");
        move.loadFromFile("images/clouds.png");
        MainMenu = AnimatedMenu(move);

        flag.loadFromFile("images/flag.png");
        koopa.loadFromFile("images/enemies.png");
        items.loadFromFile("images/items.png");

        music.openFromFile("sounds/music.ogg");
        buffer.loadFromFile("sounds/jump.ogg");

        jump.setBuffer(buffer);

        fl.setTexture(flag);
        fl.setTextureRect(IntRect(10, 8, 56, 396));
        fl.setScale(0.5, 0.5);
        fl.setPosition(1300, (H - 3) * 16 - 396 * 0.5);

        start = false;
        menuNum = 0;
    }

    void init_game() {
        tile.loadFromFile("images/mario.png");
        con.loadFromFile("images/coin.png");
        heart.loadFromFile("heart.png");

        font.loadFromFile("font.ttf");
        text = Text("N", font, 25);
        text.setPosition(10, 10);
        death_str = Text("N", font, 50);
        death_str.setPosition(420, 256);
        death_str.setFillColor(Color::Black);
        death_str.setString("Press space to restart\n      Escape to exit");

        player = Player(tile, heart);

        plate.setTexture(tile);
    }

    std::string define_current_level() {
        std::string answer = "maps/level" + std::to_string(level) + ".txt";
        return answer;
    }

    void load_level() {
        enemies.clear();
        coins.clear();
        bonuses.clear();
        // загрузка карты и расстановка врагов, собираемых элементов (монетки, бонусы) по ней
        std::ifstream in(define_current_level());
        if (in.is_open()) {
            for (int i = 0; i < H; i++) {
                std::getline(in, TileMap[i]);
                for (int cell = 0; cell < W; cell++) {
                    if (TileMap[i][cell] == 'm') {
                        Goomba *goomba = new Goomba();
                        goomba->set(tile, cell * 16, i * 16, TileMap);
                        enemies.push_back(goomba);
                    }

                    if (TileMap[i][cell] == 't') {
                        KoopaTroopa *k = new KoopaTroopa();
                        k->set(koopa, cell * 16, i * 16, TileMap);
                        enemies.push_back(k);
                    }

                    if (TileMap[i][cell] == 'c') {
                        coins.push_back(new Coin(con));
                        coins[coins.size() - 1]->set(cell * 16 - 1, i * 16 - 3);
                    }

                    if (TileMap[i][cell] == 'b') {
                        Bonus *bon = new Bonus(tile);
                        bon->set(cell * 16, i * 16);
                        bonuses.push_back(bon);
                    }

                    if (TileMap[i][cell] == 'u') {
                        MushRoom* mush = new MushRoom(items, 0, font);
                        mush->set(cell * 16, i * 16);
                        mushRooms.push_back(mush);
                    }
                }
            }
        }
    }

    void init_main_menu(std::vector<void (*)(void)> &v) {
        MainMenu.setBackground(backGround, 1, 1);

        Button *play = new Button(buttons);
        play->set_position(340, 120);
        play->set_texture_rect(8, 8, 754, 315);
        play->set_scale(0.2519, 0.260317);
        play->set_trigger(v[0]);

        Button *exit = new Button(buttons);
        exit->set_position(340, 314);
        exit->set_texture_rect(42, 672, 369, 129);
        exit->set_scale(0.5176, 0.6357);
        exit->set_trigger(v[1]);

        Button *select = new Button(buttons);
        select->set_position(340, 212);
        select->set_texture_rect(789, 37, 1119, 519);
        select->set_scale(0.1707, 0.1579);
        select->set_trigger(v[2]);

        MainMenu.set_move_rect(Vector2i(168, 59), Vector2i(604, 97));
        MainMenu.set_move_position(138, 90);

        MainMenu.add_button(*play);
        MainMenu.add_button(*exit);
        MainMenu.add_button(*select);
        MainMenu.setBackground(backGround, 1, 1);

        delete play;
        delete exit;
        delete select;
    }

    void init_pause_menu(std::vector<void (*)(void)> &functions) {
        TextButton *cont = new TextButton(font);
        std::string str = "CONTINUE";
        cont->set_string(str);
        cont->set_position(380, 188);
        cont->set_color(Color::Green);
        cont->set_trigger(functions[0]);

        TextButton *restart = new TextButton(font);
        std::string help_str = "RESTART";
        restart->set_string(help_str);
        restart->set_position(388, 270);
        restart->set_color(Color::Green);
        restart->set_trigger(functions[2]);

        TextButton *exit = new TextButton(font);
        std::string str1 = "EXIT";
        exit->set_string(str1);
        exit->set_position(417, 346);
        exit->set_color(Color::Green);
        exit->set_trigger(functions[1]);

        PauseMenu.add_button(*cont);
        PauseMenu.add_button(*exit);
        PauseMenu.add_button(*restart);

        delete cont;
        delete exit;
        delete restart;
    }

    void init_finish_menu(std::vector<void (*)(void)> &functions) {
        TextButton *next = new TextButton(font);
        std::string help_str = "NEXT LEVEL";
        next->set_string(help_str);
        next->set_position(360, 188);
        next->set_color(Color::Magenta);
        next->set_trigger(functions[0]);

        TextButton *main_menu = new TextButton(font);
        std::string help_str1 = "MAIN MENU";
        main_menu->set_string(help_str1);
        main_menu->set_position(363, 288);
        main_menu->set_color(Color::Magenta);
        main_menu->set_trigger(functions[1]);

        FinishMenu.add_button(*next);
        FinishMenu.add_button(*main_menu);
    }

    // функция отображения меню в зависисмости от значения menuNum
    void display_menus(float time, RenderWindow *window) {
        if (menuNum == 0) {
            MainMenu.display(time, window);
        }

        if (menuNum == 1) {
            PauseMenu.display(window);
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                start = true;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
        }

        if (menuNum == 2) {
            FinishMenu.display(window);
        }
    }

    // функция для управления игровым персонажем при помощи клавиатуры
    void listen_keyboard(RenderWindow *window) {
        // персонажу передается скорость движения влево
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            player.dx = -0.1;
        }

        // персонажу передается скорость движения вправо
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            player.dx = 0.1;
        }

        // позволяет игроку совершать прыжки при нажатии клавиши пробел
        if (Keyboard::isKeyPressed(Keyboard::Space) && !level_finished) {
            if (player.onGround) {
                player.dy = -0.325;
                player.onGround = false;
                //jump.play();
            }
        }

        // ставит игру на паузу
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            start = false;
            window->setMouseCursorVisible(true);
            menuNum = 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }

    // обновление положения персонажей и предметов в игре
    void update_characters(float time) {
        if (!level_finished) {
            player.update(time, TileMap);
        } else {
            player.animation(time);
        }

        for (auto &i: enemies) {
            i->update(time, player, TileMap);
        }

        for (auto &coin: coins) {
            coin->update(time);
        }

        for (auto &bonus: bonuses) {
            bonus->update(time);
        }

        for (size_t i = 0; i < mushRooms.size(); i++) {
            mushRooms[i]->update(mushRooms, i, player);
        }
    }

    void check_enemy_intersection() {
        for (auto &i: enemies) {
            if (player.rect.intersects(i->rect) && i->alive) {
                if (player.dy > 0) {
                    player.dy = -0.2;
                    player.score += 100;
                    i->alive = false;
                } else if (!player.tripin) {
                    player.dy = -0.2;
                    player.Life -= 0.015;
                }
            }
        }
    }

    void check_finish(RenderWindow *window) {
        if (player.rect.left / 16 > W - 17) {
            if (!level_finished) {
                level_finished = true;
                player.dy = 0;
                player.dx = 0;
            } else if (player.rect.left > (W - 17) * 16 + 150) {
                level_finished = false;
                start = false;
                menuNum = 2;
                window->setMouseCursorVisible(true);
                if (level < 3) {
                    level++;
                } else {
                    menuNum = 0;
                }
            }
        }
    }

    void check_claim() {
        for (size_t i = 0; i < coins.size(); i++) {
            if (player.rect.intersects(coins[i]->rect)) {
                player.score += 20;
                delete coins[i];
                coins.erase(coins.begin() + i);
            }
        }

        for (size_t i = 0; i < bonuses.size(); i++) {
            FloatRect hit_box = FloatRect(player.rect.left, player.rect.top - 2, 17, 17);
            if (hit_box.intersects(bonuses[i]->rect) && player.rect.top > bonuses[i]->rect.top) {
                bonuses[i]->scored = true;
            }
        }

        for (auto &mush : mushRooms) {
            if (mush->rect.intersects(player.rect) && !mush->scored) {
                mush->activate();
                player.tripin = true;
            }
        }
    }

    void update(float time, RenderWindow *window) {
        listen_keyboard(window);

        // проверка количества жизней игрока и в случае если они закончились выводим экран смерти
        if (player.Life < 1) {
            death_str.setPosition(player.rect.left - 100 - offsetX, 256);
            menuNum = 4;
            start = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }

        update_characters(time);
        // проверка столкновения игрока с противником
        check_enemy_intersection();
        // проверка завершил игрок уровень или нет
        check_finish(window);
        // проверка того собрал игрок монетку или бонус
        check_claim();

        // управляем смещением карты относительно игрока
        if (player.rect.left > 456 && player.rect.left <= W * 16 - 456) {
            offsetX = player.rect.left - 456;
        }

        // меняем позицию финального чекпоинта по отношению к положению игрока
        fl.setPosition((W - 17) * 16 - offsetX, (H - 3) * 16 - 396 * 0.5);
    }

    // функция отрисовки карты тайлов
    void draw_map(RenderWindow *window) {
        for (int i = 0; i < H; i++) {
            for (int j = (offsetX - 208) / 16; j < (offsetX + 912) / 16; j++) {
                plate.setPosition(j * 16 - offsetX, i * 16);

                if (TileMap[i][j] == ' ' || TileMap[i][j] == '0') { continue; }

                if (TileMap[i][j] == 'l') {
                    plate.setTextureRect({0, 140, 47, 19});
                    plate.setPosition(j * 16 - offsetX, i * 16 - 3);
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'L') {
                    plate.setTextureRect({5, 106, 67, 21});
                    plate.setPosition(j * 16 - offsetX, i * 16 - 5);
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'K') {
                    plate.setTextureRect({160, 122, 63, 21});
                    plate.setPosition(j * 16 - offsetX, i * 16 - 5);
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'o') {
                    plate.setTextureRect({98, 224, 42, 31});
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'O') {
                    plate.setTextureRect({96, 160, 62, 31});
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'R') {
                    plate.setTextureRect(IntRect(96, 112, 16, 16));
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'k') {
                    plate.setTextureRect(IntRect(112, 112, 16, 16));
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'e') {
                    plate.setTextureRect(IntRect(142, 112, 16, 16));
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'z') {
                    plate.setTextureRect(IntRect(96, 7, 106, 104));
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'r') {
                    plate.setTextureRect(IntRect(64, 143, 16, 16));
                    window->draw(plate);
                    continue;
                }

                if (TileMap[i][j] == 'q') {
                    plate.setTextureRect({239, 239, 16, 16});
                    window->draw(plate);
                    continue;
                }
            }
        }

        window->draw(fl);
    }

    void check_delete_objects() {
        for (size_t i = 0; i < enemies.size(); i++) {
            if (enemies[i]->disappear > 5 || enemies[i]->rect.top >= (H - 3) * 16) {
                delete enemies[i];
                enemies.erase(enemies.begin() + i);
            }
        }

        for (size_t i = 0; i < bonuses.size(); i++) {
            if (bonuses[i]->rect.top <= bonuses[i]->max_cordY) {
                bonuses[i]->delete_from_map(TileMap);
                bonuses[i]->emplace_with_coin(coins, con);
                delete bonuses[i];
                bonuses.erase(bonuses.begin() + i);
                if (player.Life < 2 && player.Life > 1) {
                    player.Life++;
                }
            }
        }
    }

    void draw_objects(RenderWindow *window) {
        for (auto &i: enemies) {
            window->draw(i->sprite);
        }

        for (auto &i: bonuses) {
            window->draw(i->s);
        }

        for (auto &coin: coins) {
            window->draw(coin->s);
        }

        for (auto &item : mushRooms) {
            if (!item->scored) {
                window->draw(item->s);
            } else {
                item->draw_time(window);
            }
        }
    }

    void draw(RenderWindow *window) {
        check_delete_objects();
        window->clear(Color(120, 190, 255));
        draw_map(window);
        window->draw(player.sprite);
        draw_objects(window);
        player.draw_lives(window);
        player.show_score(window, text);
    }

    void clear() {
        enemies.clear();
        TileMap->clear();
    }

    // вывод экрана смерти для игрока
    void death_screen(RenderWindow *window) {
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            restart();
            menuNum = 1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            menuNum = 0;
            [[maybe_unused]] void to_main();
            to_main();
        }

        window->draw(death_str);
    }

    void restart() {
        offsetX = 0;
        load_level();
        init_game();
        player.score = 0;
        start = true;
        //music.play();
    }
};

#pragma clang diagnostic pop

RenderWindow window(VideoMode(912, 512), "test", Style::Default);
Game game;

// инициализация функций привязанных к кнопкам
void restarted() {
    offsetX = 0;
    game.init_game();
    game.load_level();
    game.player.score = 0;
    game.start = true;
    //game.music.play();
    window.setMouseCursorVisible(false);
}

void next_level() {
    offsetX = 0;
    game.init_game();
    game.load_level();
    game.start = true;
    //game.music.play();
    window.setMouseCursorVisible(false);
}

void started() {
    offsetX = 0;
    game.init_game();
    game.load_level();
    [[maybe_unused]] void to_main();
    std::vector<void (*)(void)> functions = {[]() {
        game.start = true;
        window.setMouseCursorVisible(false);
    }, to_main, restarted};
    std::vector<void (*)(void)> func = {next_level, to_main};
    game.MainMenu.clear();
    game.init_pause_menu(functions);
    game.init_finish_menu(func);
    //game.music.play();
    game.start = true;
    window.setMouseCursorVisible(false);
}

void to_main() {
    game.clear();
    std::vector<void (*)(void)> functions = {started, []() { window.close(); },
                                             []() {}};
    game.init_main_menu(functions);
    game.PauseMenu.clear();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    window.setMouseCursorVisible(true);
    game.menuNum = 0;
    offsetX = 0;
}

int main() {
    Clock clock;

    std::vector<void (*)(void)> functions = {started, []() { window.close(); },
                                             []() {}};
    game.init_main_menu(functions);
    std::destroy(functions.begin(), functions.end());

    // основной цикл программы
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= 500;

        if (time > 20) {
            time = 200;
        }

        // обработка события закрытия окна
        Event event{};
        if (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        // отслеживание игрока относительно меню, уровней и игры
        if (game.start) {
            game.update(time, &window);
            game.draw(&window);
        } else if (game.menuNum < 4) {
            game.display_menus(time, &window);
        } else {
            game.death_screen(&window);
        }

        window.display();
    }

    return 0;
}