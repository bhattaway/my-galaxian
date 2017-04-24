#ifndef MY_GALAXIAN_H
#define MY_GALAXIAN_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

void test_galaxian_title_screen();
void test_galaxian_alien();
void test_galaxian_player_ship();

class Alien
{
public:
    //Alien(int x = 0, int y = 0);

    virtual void run() = 0;
    virtual void draw(Surface &) const = 0;

/*
//protected:
    Rect rect_;

    int state_;
    int dx_;
    int dy_;
    */
};

class AquaAlien : public Alien
{
public:
    AquaAlien(int x = 0, int y = 0);

    void run();
    void draw(Surface &) const;

private:
    static Image image_;
    Rect rect_;

    int state_;
    int dx_;
    int dy_;
};

class RedAlien : public Alien
{
public:
    RedAlien(int x = 0, int y = 0);

    void run();
    void draw(Surface &) const;

private:
    Rect rect_;
    static Image image_;

    int state_;
    int dx_;
    int dy_;
};

class PlayerShip
{
public:
    PlayerShip(int x = 0, int y = W - 100);
    void draw(Surface &) const;
    void moveRight();
    void moveLeft();

private:
    Rect rect_;
    static Image image_;

    int dx_;
    int dy_;
};

#endif

