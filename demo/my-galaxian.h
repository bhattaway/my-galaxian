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

const int MAX_SCORE_DIGITS = 9;

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
class GameStats;

class Fleet
{
public:
    void init();
    void delete_alien();
    void run();
    void draw(Surface &) const;
    void do_collision(Laser [], int, GameStats &);
    void do_collision(PlayerShip &, GameStats &);
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
    Alien(int x = 0, int y = 0,
            int row = 0, int col = 0);

    virtual void run() = 0;
    virtual void draw(Surface &) const = 0;
    Rect & rect();
    int & state();
    bool & isAlive();
    int row() const;
    int col() const;
    int score() const;

protected:
    int row_;
    int col_;
    Rect rect_;

    int state_;
    bool isAlive_;
    int dx_;
    int dy_;
    int score_;
};

class AquaAlien : public Alien
{
public:
    AquaAlien(int x = 0, int y = 0,
            int row = 0, int col = 0);

    void run();
    void draw(Surface &) const;

private:
    static Image image_;
};

class RedAlien : public Alien
{
public:
    RedAlien(int x = 0, int y = 0,
            int row = 0, int col = 0);

    void run();
    void draw(Surface &) const;

private:
    static Image image_;
};

class PurpleAlien : public Alien
{
public:
    PurpleAlien(int x = 0, int y = 0,
            int row = 0, int col = 0);

    void run();
    void draw(Surface &) const;

private:
    static Image image_;
};
class YellowAlien : public Alien
{
public:
    YellowAlien(int x = 0, int y = 0,
            int row = 0, int col = 0);

    void run();
    void draw(Surface &) const;

private:
    static Image image_;
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
    void init();

    int time_of_death_;

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

class GameStats
{
public:
    GameStats();

    void draw(Surface &);

    int num_lives_;
    int score_;
    int current_level_;
    int game_state_;

    static Image ship_image_;
    Font font_;
    Image score_text_;
    Rect score_text_rect_;
    Image score_number_;
    Rect score_number_rect_;
    char score_number_char_ [MAX_SCORE_DIGITS];
    Image level_text_;
    Rect level_text_rect_;
    //Image level_number_;
    //Rect level_number_rect_;
    Rect ship_rect_;
private:
};

#endif

