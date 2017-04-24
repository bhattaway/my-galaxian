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
    Alien ** alien = new Alien*[NUM_AQUA];

    for (int i = 0; i < NUM_AQUA / 2; ++i)
    {
        alien[i] = new AquaAlien(i,i*3);
    }
    for (int i = NUM_AQUA / 2; i < NUM_AQUA; ++i)
    {
        alien[i] = new RedAlien(i,i*3);
    }

    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;

        for (int i = 0; i < NUM_AQUA; ++i)
        {
            alien[i]->run();
        }

        surface.lock();
        surface.fill(BLACK);
        for (int i = 0; i < NUM_AQUA; ++i)
        {
            alien[i]->draw(surface);
        }
        surface.unlock();
        surface.flip();

        delay(20);
        
    }
    for (int i = 0; i < NUM_AQUA; ++i)
    {
        delete alien[i];
    }
    delete [] alien;
}

void test_galaxian_player_ship()
{
    Surface surface(W,H);
    Event event;

    const int LASER_SIZE = 100;
    PlayerShip ship(0,H-32);
    Laser laser[LASER_SIZE];
    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;

        KeyPressed keypressed = get_keypressed();

        if (keypressed[LEFTARROW])
        {
            ship.moveLeft();
        }
        if (keypressed[RIGHTARROW])
        {
            ship.moveRight();
        }
        if (keypressed[SPACE])
        {
            if (getTicks() - Laser::timeOfLastLaser_ > 500)
            {
                int i = 0;
                while (laser[i].isAlive)
                {
                    ++i;
                }

                laser[i].isAlive = true;
                laser[i].rect_.x = ship.rect_.x + ship.rect_.w / 2;
                laser[i].rect_.y = ship.rect_.y - laser[i].rect_.h + 4;
                Laser::timeOfLastLaser_ = getTicks();
            }
        }
        for (int i = 0; i < LASER_SIZE; ++i)
        {
            laser[i].run();
        }

        surface.lock();
        surface.fill(BLACK);
        ship.draw(surface);
        for (int i = 0; i < LASER_SIZE; ++i)
        {
            laser[i].draw(surface);
        }
        surface.unlock();
        surface.flip();

        delay(20);
    }
}

void test_galaxian_kill_aliens()
{
    Surface surface(W,H);
    Event event;

    //init aliens
    int NUM_AQUA = 10;
    std::cin >> NUM_AQUA;
    Alien ** alien = new Alien*[NUM_AQUA];

    for (int i = 0; i < NUM_AQUA / 2; ++i)
    {
        alien[i] = new AquaAlien(i,i*3);
    }
    for (int i = NUM_AQUA / 2; i < NUM_AQUA; ++i)
    {
        alien[i] = new RedAlien(i,i*3);
    }

    //init lasers and ship
    const int LASER_SIZE = 100;
    PlayerShip ship(0,H-32);
    Laser laser[LASER_SIZE];
}

bool isCollision(const Rect & r0, const Rect & r1)
{
    int r0xp = r0.x + r0.w;
    int r1xp = r1.x + r1.w;
    bool x_overlap = (r0.x <= r1.x && r1.x <= r0xp)
                    || (r0.x <= r1xp && r1xp <= r0xp)
                    || (r1.x <= r0.x && r0.x <= r1xp);

    int r0yp = r0.y + r0.h;
    int r1yp = r1.y + r1.h;
    bool y_overlap = (r0.y <= r1.y && r1.y <= r0yp)
                    || (r0.y <= r1yp && r1yp <= r0yp)
                    || (r1.y <= r0.y && r0.y <= r1yp);

    return (x_overlap && y_overlap);
}

/*
Alien::Alien(int x, int y)
    : state_(0),
    dx_(3),
    dy_(0)
{
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;
}
*/

Image AquaAlien::image_("images/galaxian/GalaxianAquaAlien.gif");

AquaAlien::AquaAlien(int x, int y)
      : state_(0),
      isAlive_(true),
      dx_(3),
      dy_(0)
{ 
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;

}

void AquaAlien::run()
{
    if (isAlive_)
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
                if (rand() % 1000 == 0) isAlive_ = false;
                break;
        }
    }
}

void AquaAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}

Image RedAlien::image_("images/galaxian/GalaxianRedAlien.gif");

RedAlien::RedAlien(int x, int y)
      : state_(0),
      isAlive_(true),
      dx_(3),
      dy_(0)
{ 
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;

}

void RedAlien::run()
{
    if (isAlive_)
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
}

void RedAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}


Image PlayerShip::image_("images/galaxian/GalaxianGalaxip.gif");

PlayerShip::PlayerShip(int x, int y)
    : isAlive_(true),
    dx_(3),
    dy_(0)
{
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;
}

void PlayerShip::moveRight()
{
    if (isAlive_) rect_.x += dx_;
}
void PlayerShip::moveLeft()
{
    if (isAlive_) rect_.x -= dx_;
}
void PlayerShip::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}

int Laser::timeOfLastLaser_ = 0;
Laser::Laser(int x, int y)
    : dx_(0),
    dy_(-4),
    isAlive(true),
    color_(RED)
{
    rect_.x = x;
    rect_.y = y;
    rect_.w = 2;
    rect_.h = 8;
}

void Laser::run()
{
    if (isAlive)
    {
        rect_.x += dx_;
        rect_.y += dy_;
        if (rect_.y < 0 || rect_.y > H) isAlive = false;
    }
}

void Laser::draw(Surface & surface) const
{
    if (isAlive) surface.put_rect(rect_, color_);
}

