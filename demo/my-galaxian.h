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

//constants for rows and columns for the fleet
//TODO: make the fleet array dynamic
const int NUM_ROWS = 6;
const int NUM_COLS = 10;

//const for alien dimensions
const int ALIEN_W = 32;
const int ALIEN_H = 32;

void test_galaxian_title_screen();
void test_galaxian_alien();
void test_galaxian_player_ship();
void test_galaxian_kill_aliens();
void test_galaxian_starfield();
void test_galaxian_fleet();

bool isCollision(const Rect &, const Rect &);

class Alien;
class Laser;
class PlayerShip;

class Fleet
{
public:
    void init();
    void delete_alien();
    void run();
    void draw(Surface &) const;
    void do_collision(Laser [], int);
    void do_collision(PlayerShip &);
    void recalculate_num_aliens_alive();
    void switch_state(int);
    Rect & rect();

    Alien * alien[NUM_ROWS][NUM_COLS];
    int num_aliens_alive;
    int fleet_state;
    Rect rect_;
    int dx_;
    int time_of_fleet_death_;
    int time_of_last_attack_;
private:
};

class Alien
{
public:
    //Alien(int x = 0, int y = 0);

    virtual void run() = 0;
    virtual void draw(Surface &) const = 0;
    virtual bool & isAlive() = 0;
    virtual Rect & rect() = 0;
    virtual int & state() = 0;

protected:
    int row_;
    int col_;
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
    AquaAlien(int x = 0, int y = 0,
            int row = 0, int col = 0);

    void run();
    void draw(Surface &) const;
    bool & isAlive();
    Rect & rect();
    int & state();

private:
    Rect rect_;
    static Image image_;

    int state_;
    bool isAlive_;
    int dx_;
    int dy_;
};

class RedAlien : public Alien
{
public:
    RedAlien(int x = 0, int y = 0);

    void run();
    void draw(Surface &) const;
    bool & isAlive();
    Rect & rect();
    int & state();

private:
    Rect rect_;
    static Image image_;

    int state_;
    bool isAlive_;
    int dx_;
    int dy_;
};

class PurpleAlien : public Alien
{
public:
    PurpleAlien(int x = 0, int y = 0);

    void run();
    void draw(Surface &) const;
    bool & isAlive();
    Rect & rect();
    int & state();

private:
    Rect rect_;
    static Image image_;

    int state_;
    bool isAlive_;
    int dx_;
    int dy_;
};
class YellowAlien : public Alien
{
public:
    YellowAlien(int x = 0, int y = 0);

    void run();
    void draw(Surface &) const;
    bool & isAlive();
    Rect & rect();
    int & state();

private:
    Rect rect_;
    static Image image_;

    int state_;
    bool isAlive_;
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
    bool & isAlive();
    Rect & rect();

private:
    Rect rect_;
    static Image image_;

    bool isAlive_;
    int dx_;
    int dy_;
};

class Laser
{
public:
    Laser(int x = 0, int y = 0);
    void run();
    void draw(Surface &) const;
    bool & isAlive();
    Rect & rect();
    
    static int timeOfLastLaser_;
private:
    bool isAlive_;
    Rect rect_;
    Color color_;

    int dx_;
    int dy_;
};

class Explosion
{
public:
    Explosion();
    void set(int x, int y);
    void run();
    void draw(Surface &) const;
    bool & isAlive();

private:
    bool isAlive_;
    int x_;
    int y_;
    int r_;
    int dr_;
};

class Star
{
public:
    Star(int x = 0, int y = 0);
    void run();
    void draw(Surface &) const;
private:
    Rect rect_;
    Color color_;

    int dx_;
    int dy_;
    int dcolor_;
};

#endif

