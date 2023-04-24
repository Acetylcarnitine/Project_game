#pragma once
#ifndef HELLOSFML_CONSTANTS_H
#define HELLOSFML_CONSTANTS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <fstream>
#include <vector>
#include <thread>
#include <cmath>
#include <random>

using namespace sf;

// модификатор extern дает доступ к переменной в других файлах
// и позволяет разделять объявление и инициализацию переменной
extern float offsetX;

// размеры карты
const int H = 32;
const int W = 200;

template<typename T>
int sign_of(T variable);

int random_from(int left_range, int right_range);

#endif //HELLOSFML_CONSTANTS_H