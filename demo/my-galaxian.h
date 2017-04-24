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

class AquaAlien
{
public:
    AquaAlien(int x = 0, int y = 0);

    void run();
    void draw(Surface &);

private:
    Rect rect_;
    static Image image_;

    int state_;
    int dx_;
    int dy_;
    
};

class RedAlien
{
public:
    RedAlien(int x = 0, int y = 0);

    void run();
    void draw(Surface &);

private:
    Rect rect_;
    static Image image_;

    int state_;
    int dx_;
    int dy_;
};

#endif

