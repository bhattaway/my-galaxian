#include "my-galaxian.h"


void test_galaxian_title_screen()
{
    Surface surface(W, H);
    Event event;
    Font font("fonts/FreeMono.ttf", 24);
    Image image(font.render("WELCOME TO MY-GALAXIAN press space", WHITE));
    Image image2(font.render("game", WHITE));
    Rect rect = image.getRect();

    int RATE = 1000/60;

    bool b = false;
    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;

        int start = getTicks();

        KeyPressed keypressed = get_keypressed();
        if (keypressed[SPACE])
        {
            b = true;
        }

        surface.lock();
        surface.fill(BLACK);
        (b ? surface.put_image(image2, rect) : surface.put_image(image, rect));
        surface.unlock();
        surface.flip();

        int end = getTicks();

        int dt = RATE - end + start;
        if (dt > 0) delay(dt);
    }
    return;
}

void test_galaxian_alien()
{
    Surface surface(W,H);
    Event event;

    int NUM_AQUA = 10;
    std::cin >> NUM_AQUA;
    AquaAlien ** aqua = new AquaAlien*[NUM_AQUA];

    for (int i = 0; i < NUM_AQUA; ++i)
    {
        aqua[i] = new AquaAlien(i,i*5);
    }

    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;

        for (int i = 0; i < NUM_AQUA; ++i)
        {
            aqua[i]->run();
        }

        surface.lock();
        surface.fill(BLACK);
        for (int i = 0; i < NUM_AQUA; ++i)
        {
            aqua[i]->draw(surface);
        }
        surface.unlock();
        surface.flip();

        delay(20);
        
    }
    for (int i = 0; i < NUM_AQUA; ++i)
    {
        delete aqua[i];
    }
    delete [] aqua;
}

Image AquaAlien::image_("images/galaxian/GalaxianAquaAlien.gif");

AquaAlien::AquaAlien(int x, int y)
      : state_(0),
      dx_(3),
      dy_(0)
{ 
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;

}

void AquaAlien::run()
{
    switch (state_)
    {
        case 0: //passive in fleet
            rect_.x += dx_;
            if (rect_.x < 0)
            {
                rect_.x = 0;
                dx_ = -dx_;
            }
            else if ((rect_.x + rect_.w) > (W - 1))
            {
                rect_.x = W - 1 - rect_.w;
                dx_ = -dx_;
            }

            if (rand() % 100 == 0) state_ = 1;
            break;
        case 1: //attack
            dy_ = 3;
            rect_.y += dy_;
            if (rect_.y > H)
            {
                rect_.y = 0;
                state_ = 0;
            }
            break;
    }
}

void AquaAlien::draw(Surface & surface) 
{
    surface.put_image(image_, rect_);
}

