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
                while (laser[i].isAlive())
                {
                    ++i;
                }

                laser[i].isAlive() = true;
                laser[i].rect().x = ship.rect().x + ship.rect().w / 2;
                laser[i].rect().y = ship.rect().y - laser[i].rect().h + 4;
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

    for (int i = 0; i < NUM_AQUA / 4; ++i)
    {
        alien[i] = new AquaAlien(i,i*3);
    }
    for (int i = NUM_AQUA / 4; i < NUM_AQUA / 2; ++i)
    {
        alien[i] = new RedAlien(i,i*3);
    }
    for (int i = NUM_AQUA / 2; i < NUM_AQUA * 3 / 4; ++i)
    {
        alien[i] = new PurpleAlien(i,i*3);
    }
    for (int i = NUM_AQUA * 3 / 4; i < NUM_AQUA; ++i)
    {
        alien[i] = new YellowAlien(i,i*3);
    }

    //init lasers and ship
    const int LASER_SIZE = 100;
    PlayerShip ship(0,H-32);
    Laser laser[LASER_SIZE];

    //init explosion
    const int NUM_EXPLOSIONS = 100;
    Explosion explosion[NUM_EXPLOSIONS];

    //init stars
    const int NUM_STARS = 50;
    Star star[NUM_STARS];

    const int FRAME_RATE = 1000 / 30;

    //game loop
    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;
        int frame_start = getTicks();
        //handle input
        KeyPressed keypressed = get_keypressed();

        if (keypressed[LEFTARROW])
        {
            ship.moveLeft();
        }
        if (keypressed[RIGHTARROW])
        {
            ship.moveRight();
        }
        if (keypressed[SPACE] && ship.isAlive())
        {
            if (getTicks() - Laser::timeOfLastLaser_ > 500)
            {
                int i = 0;
                while (laser[i].isAlive())
                {
                    ++i;
                }

                laser[i].isAlive() = true;
                laser[i].rect().x = ship.rect().x + ship.rect().w / 2;
                laser[i].rect().y = ship.rect().y - laser[i].rect().h + 4;
                Laser::timeOfLastLaser_ = getTicks();
            }
        }
        //run stuff
        for (int i = 0; i < NUM_STARS; ++i)
        {
            star[i].run();
        }
        for (int i = 0; i < LASER_SIZE; ++i)
        {
            laser[i].run();
        }
        for (int i = 0; i < NUM_AQUA; ++i)
        {
            alien[i]->run();
        }
        for (int i = 0; i < NUM_EXPLOSIONS; ++i)
        {
            explosion[i].run();
        }

        //check collision of lasers vs aliens
        for (int i = 0; i < LASER_SIZE; ++i)
        {
            if (laser[i].isAlive())
            {
                for (int j = 0; j < NUM_AQUA; ++j)
                {
                    if (alien[j]->isAlive())
                    {
                        if (isCollision(laser[i].rect(), alien[j]->rect()))
                        {
                            int k = 0;
                            while (explosion[k].isAlive())
                                ++k;

                            explosion[k].set(alien[j]->rect().x + alien[j]->rect().w / 2,
                                             alien[j]->rect().y + alien[j]->rect().h / 2);
                            alien[j]->isAlive() = false;
                            laser[i].isAlive() = false;
                        }
                    }
                }
            }
        }

        //check collision of aliens vs player ship
        for (int i = 0; i < NUM_AQUA; ++i)
        {
            if (alien[i]->isAlive())
            {
                if (isCollision(alien[i]->rect(), ship.rect()) && ship.isAlive())
                {
                    int k = 0;
                    while (explosion[k].isAlive())
                        ++k;

                    explosion[k].set(alien[i]->rect().x + alien[i]->rect().w / 2, 
                                     alien[i]->rect().y + alien[i]->rect().h / 2);

                    k = 0;
                    while (explosion[k].isAlive())
                        ++k;

                    //explosion for player ship
                    explosion[k].set(ship.rect().x + ship.rect().w / 2, 
                                     ship.rect().y + ship.rect().h / 2);

                    alien[i]->isAlive() = false;
                    ship.isAlive() = false;
                }
            }
        }

        surface.lock();
        surface.fill(BLACK);
        //draw stuff
        for (int i = 0; i < NUM_STARS; ++i)
        {
            star[i].draw(surface);
        }
        ship.draw(surface);
        for (int i = 0; i < LASER_SIZE; ++i)
        {
            laser[i].draw(surface);
        }
        for (int i = 0; i < NUM_AQUA; ++i)
        {
            alien[i]->draw(surface);
        }
        for (int i = 0; i < NUM_EXPLOSIONS; ++i)
        {
            explosion[i].draw(surface);
        }
        surface.unlock();
        surface.flip();

        int frame_end = getTicks();
        int delaytime = FRAME_RATE - frame_end + frame_start;
        if (delaytime > 0) delay(delaytime);
        
    }

    //deleting memory
    for (int i = 0; i < NUM_AQUA; ++i)
    {
        delete alien[i];
    }
    delete [] alien;
}

void test_galaxian_starfield()
{
    Surface surface(W,H);
    Event event;

    //init stars
    const int NUM_STARS = 50;
    Star star[NUM_STARS];
    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;
        for (int i = 0; i < NUM_STARS; ++i)
        {
            star[i].run();
        }

        surface.lock();
        surface.fill(BLACK);
        for (int i = 0; i < NUM_STARS; ++i)
        {
            star[i].draw(surface);
        }
        surface.unlock();
        surface.flip();

        delay(20);

    }
}

void test_galaxian_fleet()
{
    Surface surface(W,H);
    Event event;
    Fleet fleet;

    fleet.init();

    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;

        fleet.run();

        surface.lock();
        surface.fill(BLACK);
        fleet.draw(surface);
        surface.unlock();
        surface.flip();

        delay(30);
    }

    fleet.delete_alien();
}

bool isCollision(const Rect & r0, const Rect & r1)
{
    int r0xp = r0.x + r0.w; //stands for r0.x(prime)
    int r1xp = r1.x + r1.w;
    /*
    bool x_overlap = (r0.x <= r1.x && r1.x <= r0xp)
                    || (r0.x <= r1xp && r1xp <= r0xp)
                    || (r1.x <= r0.x && r0.x <= r1xp);
                    */

    int r0yp = r0.y + r0.h;
    int r1yp = r1.y + r1.h;
    /*
    bool y_overlap = (r0.y <= r1.y && r1.y <= r0yp)
                    || (r0.y <= r1yp && r1yp <= r0yp)
                    || (r1.y <= r0.y && r0.y <= r1yp);
                    */

    return (((r0.x <= r1.x && r1.x <= r0xp) //x_overlap
                    || (r0.x <= r1xp && r1xp <= r0xp)
                    || (r1.x <= r0.x && r0.x <= r1xp))
            && ((r0.y <= r1.y && r1.y <= r0yp) //y_overlap
                    || (r0.y <= r1yp && r1yp <= r0yp)
                    || (r1.y <= r0.y && r0.y <= r1yp)));
    //return (x_overlap && y_overlap);
}

void Fleet::init()
{
    for (int col = 0; col < 10; ++col)
    {
        alien[0][col] = new YellowAlien(32 * col, 0 * 32);
    }
    for (int col = 0; col < 10; ++col)
    {
        alien[1][col] = new RedAlien(32 * col, 1 * 32);
    }
    for (int col = 0; col < 10; ++col)
    {
        alien[2][col] = new PurpleAlien(32 * col, 2 * 32);
    }
    for (int row = 3; row < 6; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            alien[row][col] = new AquaAlien(32 * col, 32 * row);
        }
    }
}

void Fleet::delete_alien()
{
    for (int row = 0; row < 6; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            delete alien[row][col];
        }
    }
}
void Fleet::run()
{
    if (alien[0][0]->rect().x < 10)
    {
        for (int row = 0; row < 6; ++row)
        {
            for (int col = 0; col < 10; ++col)
            {
                alien[row][col]->state() = 0;
            }
        }
    }
    if (alien[0][9]->rect().x + 32 > W - 10)
    {
        for (int row = 0; row < 6; ++row)
        {
            for (int col = 0; col < 10; ++col)
            {
                alien[row][col]->state() = 1;
            }
        }
    }
    for (int row = 0; row < 6; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            alien[row][col]->run();
        }
    }
}
void Fleet::draw(Surface & surface) const
{
    for (int row = 0; row < 6; ++row)
    {
        for (int col = 0; col < 10; ++col)
        {
            alien[row][col]->draw(surface);
        }
    }
}


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
            case 0: //passive in fleet, moving right
                rect_.x += dx_;
                if (rect_.x < 0)
                {
                    rect_.x = 0;
                    //dx_ = -dx_;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                    //dx_ = -dx_;
                }

                //if (rand() % 100 == 0) state_ = 1;
                break;
            case 1: //passive in fleet, moving left
                rect_.x -= dx_;

                if (rect_.x < 0)
                {
                    rect_.x = 0;
                    //dx_ = -dx_;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                    //dx_ = -dx_;
                }
                break;
            case 2: //attack
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

void AquaAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}
bool & AquaAlien::isAlive()
{
    return isAlive_;
}
Rect & AquaAlien::rect()
{
    return rect_;
}
int & AquaAlien::state()
{
    return state_;
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
            case 0: //passive in fleet, moving right
                rect_.x += dx_;
                if (rect_.x < 0)
                {
                    rect_.x = 0;
                    //dx_ = -dx_;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                    //dx_ = -dx_;
                }

                //if (rand() % 100 == 0) state_ = 1;
                break;
            case 1: //passive in fleet, moving left
                rect_.x -= dx_;

                if (rect_.x < 0)
                {
                    rect_.x = 0;
                    //dx_ = -dx_;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                    //dx_ = -dx_;
                }
                break;
            case 2: //attack
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
bool & RedAlien::isAlive()
{
    return isAlive_;
}
Rect & RedAlien::rect()
{
    return rect_;
}
int & RedAlien::state()
{
    return state_;
}

Image PurpleAlien::image_("images/galaxian/GalaxianPurpleAlien.gif");

PurpleAlien::PurpleAlien(int x, int y)
      : state_(0),
      isAlive_(true),
      dx_(3),
      dy_(0)
{ 
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;

}

void PurpleAlien::run()
{
    if (isAlive_)
    {
        switch (state_)
        {
            case 0: //passive in fleet, moving right
                rect_.x += dx_;
                if (rect_.x < 0)
                {
                    rect_.x = 0;
                    //dx_ = -dx_;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                    //dx_ = -dx_;
                }

                //if (rand() % 100 == 0) state_ = 1;
                break;
            case 1: //passive in fleet, moving left
                rect_.x -= dx_;

                if (rect_.x < 0)
                {
                    rect_.x = 0;
                    //dx_ = -dx_;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                    //dx_ = -dx_;
                }
                break;
            case 2: //attack
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

void PurpleAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}
bool & PurpleAlien::isAlive()
{
    return isAlive_;
}
Rect & PurpleAlien::rect()
{
    return rect_;
}
int & PurpleAlien::state()
{
    return state_;
}

Image YellowAlien::image_("images/galaxian/GalaxianFlagship.gif");

YellowAlien::YellowAlien(int x, int y)
      : state_(0),
      isAlive_(true),
      dx_(3),
      dy_(0)
{ 
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;

}

void YellowAlien::run()
{
    if (isAlive_)
    {
        switch (state_)
        {
            case 0: //passive in fleet, moving right
                rect_.x += dx_;
                if (rect_.x < 0)
                {
                    rect_.x = 0;
                    //dx_ = -dx_;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                    //dx_ = -dx_;
                }

                //if (rand() % 100 == 0) state_ = 1;
                break;
            case 1: //passive in fleet, moving left
                rect_.x -= dx_;

                if (rect_.x < 0)
                {
                    rect_.x = 0;
                    //dx_ = -dx_;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                    //dx_ = -dx_;
                }
                break;
            case 2: //attack
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

void YellowAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}
bool & YellowAlien::isAlive()
{
    return isAlive_;
}
Rect & YellowAlien::rect()
{
    return rect_;
}
int & YellowAlien::state()
{
    return state_;
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
    if (isAlive_)
    {
        rect_.x += dx_;
        if (rect_.x + rect_.w > W) rect_.x = W - rect_.w;
    }
}
void PlayerShip::moveLeft()
{
    if (isAlive_)
    {
        rect_.x -= dx_;
        if (rect_.x < 0) rect_.x = 0;
    }
}
void PlayerShip::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}
bool & PlayerShip::isAlive()
{
    return isAlive_;
}
Rect & PlayerShip::rect()
{
    return rect_;
}

int Laser::timeOfLastLaser_ = 0;
Laser::Laser(int x, int y)
    : dx_(0),
    dy_(-4),
    isAlive_(true),
    color_(RED)
{
    rect_.x = x;
    rect_.y = y;
    rect_.w = 2;
    rect_.h = 8;
}

void Laser::run()
{
    if (isAlive_)
    {
        rect_.x += dx_;
        rect_.y += dy_;
        if (rect_.y < 0 || rect_.y > H) isAlive_ = false;
    }
}

void Laser::draw(Surface & surface) const
{
    if (isAlive_) surface.put_rect(rect_, color_);
}
bool & Laser::isAlive()
{
    return isAlive_;
}
Rect & Laser::rect()
{
    return rect_;
}

Explosion::Explosion()
    : isAlive_(false),
    x_(0),
    y_(0),
    r_(1),
    dr_(1)
{ }

void Explosion::set(int x, int y)
{
    x_ = x;
    y_ = y;
    r_ = 1;
    isAlive_ = true;
}
void Explosion::run()
{
    if (isAlive_)
    {
        r_ += dr_;
        if (r_ > 30) isAlive_ = false;
    }
}
void Explosion::draw(Surface & surface) const
{
    if (isAlive_)
    {
        surface.put_circle(x_, y_, r_, RED);
        surface.put_circle(x_, y_, r_ / 1.2, YELLOW);
        surface.put_circle(x_, y_, r_ / 2, WHITE);
    }
}
bool & Explosion::isAlive()
{
    return isAlive_;
}

Star::Star(int x, int y)
    : dx_(0),
    dy_(rand() % 4 + 1),
    color_(rand_color())
{
    rect_.x = rand() % W;
    rect_.y = rand() % H;
    rect_.w = 1;
    rect_.h = 1;
}
void Star::run()
{
    rect_.y += dy_;
    if (rect_.y > H) 
    {
        rect_.x = rand() % W;
        rect_.y = 0;
        dy_ = rand() % 4 + 1;
        color_ = rand_color();
    }
    //color_.R;
    //color_.G += dG_;
    //color_.B += dB_;
}
void Star::draw(Surface & surface) const
{
    surface.put_rect(rect_, color_);
}

