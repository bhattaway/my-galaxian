#include "my-galaxian.h"

int Laser::timeOfLastLaser_ = 0;

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

    //init fleet
    fleet.init();
    //init lasers and ship
    const int LASER_SIZE = 100;
    PlayerShip ship(0, 0);
    //ship.init();
    Laser laser[LASER_SIZE];

    //init explosion
    const int NUM_EXPLOSIONS = 100;
    Explosion explosion[NUM_EXPLOSIONS];

    //init stars
    const int NUM_STARS = 50;
    Star star[NUM_STARS];

    const int FRAME_RATE = 1000 / 30;

    GameStats gamestats;

    Font font1("fonts/FreeMono.ttf", 24);

    Image title_screen(font1.render("WELCOME TO GALAXIAN", GREEN));
    Rect title_screen_rect = title_screen.getRect();
    title_screen_rect.x = W / 2;
    title_screen_rect.y = H / 2;

    Image game_over(font1.render("GAME OVER", RED));
    Rect game_over_rect = game_over.getRect();
    game_over_rect.x = W / 2;
    game_over_rect.y = H / 2;

    //game loop
    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;

        int frame_start = getTicks();
        //handle input
        KeyPressed keypressed = get_keypressed();

        switch (gamestats.game_state_)
        {
    case 0:

        if (keypressed[SPACE])
        {
            gamestats.game_state_ = 1; //init state
        }
        for (int i = 0; i < NUM_STARS; ++i)
        {
            star[i].run();
        }
        surface.lock();
        surface.fill(BLACK);
        surface.put_image(title_screen, title_screen_rect);
        for (int i = 0; i < NUM_STARS; ++i)
        {
            star[i].draw(surface);
        }
        surface.unlock();
        surface.flip();
        break;
    case 1: //init state
        fleet.init();
        ship.init();
        gamestats.init();
        gamestats.game_state_ = 2;
        break;

    case 2: //game loop
        if (ship.isAlive())
        {
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

                    laser[i].set(ship.rect().x + ship.rect().w / 2,
                                ship.rect().y - laser[i].rect().h,
                                -8);
                    Laser::timeOfLastLaser_ = getTicks();
                }
            }
        }
        else
        {
            if (ship.time_of_death_ == 0)
            {
                ship.time_of_death_ = getTicks();
            }
            if (getTicks() - ship.time_of_death_ > 3000)
            {
                if (gamestats.num_lives_ > 0)
                {
                    ship.init();
                }
                else
                {
                    gamestats.game_state_ = 0; //title screen
                }
            }
        }

        //run stuff
        fleet.run();
        for (int i = 0; i < NUM_STARS; ++i)
        {
            star[i].run();
        }
        for (int i = 0; i < LASER_SIZE; ++i)
        {
            laser[i].run();
        }
        for (int i = 0; i < NUM_EXPLOSIONS; ++i)
        {
            explosion[i].run();
        }

        //handle collisions: lasers vs aliens
        fleet.do_collision(laser, LASER_SIZE, 
                explosion, NUM_EXPLOSIONS, gamestats);
        //collisions: aliens vs playership
        fleet.do_collision(ship, explosion, NUM_EXPLOSIONS, gamestats);

        surface.lock();
        surface.fill(BLACK);
        //draw stuff
        gamestats.draw(surface);
        for (int i = 0; i < NUM_STARS; ++i)
        {
            star[i].draw(surface);
        }
        ship.draw(surface);
        for (int i = 0; i < LASER_SIZE; ++i)
        {
            laser[i].draw(surface);
        }
        for (int i = 0; i < NUM_EXPLOSIONS; ++i)
        {
            explosion[i].draw(surface);
        }
        fleet.draw(surface);
        if (gamestats.num_lives_ == 0)
        {
            surface.put_image(game_over, game_over_rect);
        }
        surface.unlock();
        surface.flip();

        }//switch statement end

        int frame_end = getTicks();
        int delaytime = FRAME_RATE - frame_end + frame_start;
        if (delaytime > 0) delay(delaytime);
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

int Fleet::fleet_state = 0;
Rect Fleet::rect_;
void Fleet::init()
{
    for (int col = 0; col < NUM_COLS; ++col)
    {
        alien[0][col] = new YellowAlien(32 * col, 1 * 32, 0, col);
    }
    for (int col = 0; col < NUM_COLS; ++col)
    {
        alien[1][col] = new RedAlien(32 * col, 2 * 32, 1, col);
    }
    for (int col = 0; col < NUM_COLS; ++col)
    {
        alien[2][col] = new PurpleAlien(32 * col, 3 * 32, 2, col);
    }
    for (int row = 3; row < NUM_ROWS; ++row)
    {
        for (int col = 0; col < NUM_COLS; ++col)
        {
            alien[row][col] = new AquaAlien(32 * col, 32 * row + 32, row, col);
        }
    }
    alien[0][0]->isAlive() = false;
    alien[0][1]->isAlive() = false;
    alien[0][2]->isAlive() = false;
    alien[0][4]->isAlive() = false;
    alien[0][5]->isAlive() = false;
    alien[0][7]->isAlive() = false;
    alien[0][8]->isAlive() = false;
    alien[0][9]->isAlive() = false;
    alien[1][0]->isAlive() = false;
    alien[1][1]->isAlive() = false;
    alien[1][8]->isAlive() = false;
    alien[1][9]->isAlive() = false;
    alien[2][0]->isAlive() = false;
    alien[2][9]->isAlive() = false;

    switch_state(0);

    rect_.w = alien[0][0]->rect().w * NUM_COLS;
    rect_.h = alien[0][0]->rect().h * NUM_ROWS;
    rect_.x = 10;
    dx_ = 2;
    recalculate_num_aliens_alive();

    time_of_fleet_death_ = 0;
    time_of_last_attack_ = getTicks();
}

void Fleet::delete_alien()
{
    for (int row = 0; row < NUM_ROWS; ++row)
    {
        for (int col = 0; col < NUM_COLS; ++col)
        {
            delete alien[row][col];
        }
    }
}
void Fleet::run()
{
    if (num_aliens_alive != 0)
    {
        if (rect_.x < 10)
        {
            switch_state(0);
            dx_ = -dx_;
        }
        if (rect_.x + rect_.w > W)
        {
            switch_state(1);
            dx_ = -dx_;
        }
        if (getTicks() - time_of_last_attack_ > 1250)
        {
            int randrow = rand() % NUM_ROWS;
            int randcol = rand() % NUM_COLS;
            while (!alien[randrow][randcol]->isAlive())
            {
                randrow = rand() % NUM_ROWS;
                randcol = rand() % NUM_COLS;
            }
            alien[randrow][randcol]->state() = 3; //prepare attack state
            if (randrow == 0) //if the yellow alien is chosen
                              //make the red aliens below it attack too
            {
                alien[randrow + 1][randcol]->state() = 3;
                alien[randrow + 1][randcol - 1]->state() = 3;
                alien[randrow + 1][randcol + 1]->state() = 3;
            }
            time_of_last_attack_ = getTicks();
        }
        for (int row = 0; row < NUM_ROWS; ++row)
        {
            for (int col = 0; col < NUM_COLS; ++col)
            {
                alien[row][col]->run();
            }
        }

        rect_.x += dx_;
        //std::cout << rect_ << std::endl;
    }
    else
    {
        if(time_of_fleet_death_ == 0)
        {
            delete_alien();
            time_of_fleet_death_ = getTicks();
        }
        if(getTicks() - time_of_fleet_death_ > 5000)
        {
            init();
        }
    }
}
void Fleet::draw(Surface & surface) const
{
    if (num_aliens_alive != 0)
    {
        for (int row = 0; row < NUM_ROWS; ++row)
        {
            for (int col = 0; col < NUM_COLS; ++col)
            {
                alien[row][col]->draw(surface);
            }
        }
    }
}
void Fleet::do_collision(Laser laser[], int laser_size, 
        Explosion explosion [], int explosion_size, GameStats & gamestats)
{
    if (num_aliens_alive != 0)
    {
        //each laser vs each alien
        for (int i = 0; i < laser_size; ++i)
        {
            if (laser[i].isAlive())
            {
                for (int row = 0; row < NUM_ROWS; ++row)
                {
                    for (int col = 0; col < NUM_COLS; ++col)
                    {
                        if (alien[row][col]->isAlive() 
                          && isCollision(laser[i].rect(), alien[row][col]->rect()))
                        {
                            int k = 0;
                            while (explosion[k].isAlive())
                                ++k;

                            explosion[k].set(alien[row][col]->rect().x 
                                    + alien[row][col]->rect().w / 2,
                                    alien[row][col]->rect().y 
                                    + alien[row][col]->rect().h / 2);

                            gamestats.score_ += alien[row][col]->score();
                            alien[row][col]->isAlive() = false;
                            laser[i].isAlive() = false;
                            recalculate_num_aliens_alive();
                        }
                    }
                }
            }
        }
    }
}
void Fleet::do_collision(PlayerShip & ship, Explosion explosion[],
       int explosion_size, GameStats & gamestats)
{
    if (num_aliens_alive != 0)
    {
        //check collision of aliens vs player ship
        for (int row = 0; row < NUM_ROWS; ++row)
        {
            for (int col = 0; col < NUM_COLS; ++col)
            {
                if (alien[row][col]->isAlive()
                   && alien[row][col]->state() == 2
                   && isCollision(alien[row][col]->rect(), ship.rect())
                   && ship.isAlive())
                {
                    int k = 0;
                    while (explosion[k].isAlive())
                        ++k;

                    explosion[k].set(alien[row][col]->rect().x 
                            + alien[row][col]->rect().w / 2, 
                                     alien[row][col]->rect().y 
                                     + alien[row][col]->rect().h / 2);

                    k = 0;
                    while (explosion[k].isAlive())
                        ++k;

                    //explosion for player ship
                    explosion[k].set(ship.rect().x + ship.rect().w / 2, 
                                     ship.rect().y + ship.rect().h / 2);
                                     

                    alien[row][col]->isAlive() = false;
                    gamestats.score_ += alien[row][col]->score();

                    ship.isAlive() = false;
                    --gamestats.num_lives_;
                    recalculate_num_aliens_alive();
                }
            }
        }
    }
}
void Fleet::recalculate_num_aliens_alive()
{
    num_aliens_alive = 0;
    for (int row = 0; row < NUM_ROWS; ++row)
    {
        for (int col = 0; col < NUM_COLS; ++col)
        {
            if (alien[row][col]->isAlive()) ++num_aliens_alive;
        }
    }
}
void Fleet::switch_state(int new_state)
{
    for (int row = 0; row < NUM_ROWS; ++row)
    {
        for (int col = 0; col < NUM_COLS; ++col)
        {
            if (alien[row][col]->state() != 2) //2 is attack state
            {
                alien[row][col]->state() = new_state;
            }
        }
    }
    fleet_state = new_state;
}
Rect & Fleet::rect()
{
    return rect_;
}

Alien::Alien(int x, int y, int row, int col)
    : row_(row),
    col_(col),
    state_(0),
    isAlive_(true),
    dx_(2),
    dy_(4)
{
    laser_.isAlive() = false;
    rect_.x = x;
    rect_.y = y;
}
Rect & Alien::rect()
{
    return rect_;
}
int & Alien::state()
{
    return state_;
}
bool & Alien::isAlive()
{
    return isAlive_;
}
int Alien::row() const
{
    return row_;
}
int Alien::col() const
{
    return col_;
}
int Alien::score() const
{
    return score_;
}

Image AquaAlien::image_("images/galaxian/GalaxianAquaAlien.gif");

AquaAlien::AquaAlien(int x, int y, int row, int col)
    : Alien(x, y, row, col)
{ 
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;
    score_ = 30;
}

void AquaAlien::run()
{
    if (isAlive_)
    {
        switch (state_)
        {
            case 0: //passive in fleet, moving right
                rect_.x += dx_;
                if (rect_.y < row_ * 32 + 32)
                {
                    rect_.y += dy_;
                }
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
                if (rect_.y < row_ * 32 + 32)
                {
                    rect_.y += dy_;
                }
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
                rect_.y += dy_;
                if (rect_.x - 50 < destination_x_
                        && destination_x_ < rect_.x + 50)
                {
                    dx_ = 1;
                }
                else if (rect_.x - 100 < destination_x_
                        && destination_x_ < rect_.x + 100)
                {
                    dx_ = 3;
                }
                else if (rect_.x - 250 < destination_x_
                        && destination_x_ < rect_.x + 250)
                {
                    dx_ = 4;
                }
                if (destination_x_ > rect_.x)
                {
                    rect_.x += dx_;
                }
                else
                {
                    rect_.x -= dx_;
                }
                if (rect_.y > H)
                {
                    dx_ = 2;
                    rect_.x = 32 * col_ + Fleet::rect_.x - 10;
                    rect_.y = 32;
                    state_ = Fleet::fleet_state;
                }
                break;
            case 3: //prepare attack
                destination_x_ = rand() % (W - 32);
                state_ = 2;
                break;
        }
    }
    laser_.run();
}

void AquaAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
    laser_.draw(surface);
}

Image RedAlien::image_("images/galaxian/GalaxianRedAlien.gif");

RedAlien::RedAlien(int x, int y, int row, int col)
    : Alien(x, y, row, col)
{ 
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;
    score_ = 50;
}

void RedAlien::run()
{
    if (isAlive_)
    {
        switch (state_)
        {
            case 0: //passive in fleet, moving right
                rect_.x += dx_;
                if (rect_.y < row_ * 32 + 32)
                {
                    rect_.y += dy_;
                }
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
                if (rect_.y < row_ * 32 + 32)
                {
                    rect_.y += dy_;
                }

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
                rect_.y += dy_;
                if (rect_.x - 75 < destination_x_
                        && destination_x_ < rect_.x + 75)
                {
                    dx_ = 1;
                }
                if (rect_.x < destination_x_)
                {
                    rect_.x += dx_;
                }
                else
                {
                    rect_.x -= dx_;
                }
                if (rect_.y > H)
                {
                    dx_ = 2;
                    rect_.x = 32 * col_ + Fleet::rect_.x - 10;
                    rect_.y = 32;
                    state_ = Fleet::fleet_state;
                }
                break;
            case 3: //prepare attack
                destination_x_ = getTicks() % (W - 32);

                state_ = 2;
                break;
        }
    }
}

void RedAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}

Image PurpleAlien::image_("images/galaxian/GalaxianPurpleAlien.gif");

PurpleAlien::PurpleAlien(int x, int y, int row, int col)
    : Alien(x, y, row, col)
{ 
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;
    score_ = 40;
}

void PurpleAlien::run()
{
    if (isAlive_)
    {
        switch (state_)
        {
            case 0: //passive in fleet, moving right
                rect_.x += dx_;
                if (rect_.y < row_ * 32 + 32)
                {
                    rect_.y += dy_;
                }
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
                if (rect_.y < row_ * 32 + 32)
                {
                    rect_.y += dy_;
                }

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
                rect_.y += dy_;
                if (rect_.x - 10 < destination_x_
                        && destination_x_ < rect_.x + 10)
                {
                    destination_x_ = rand() % (W - 32);
                }
                if (rect_.x - 50 < destination_x_
                        && destination_x_ < rect_.x + 50)
                {
                    dx_ = 2;
                }
                else if (rect_.x - 100 < destination_x_
                        && destination_x_ < rect_.x + 100)
                {
                    dx_ = 3;
                }
                else if (rect_.x - 250 < destination_x_
                        && destination_x_ < rect_.x + 250)
                {
                    dx_ = 4;
                }
                if (destination_x_ > rect_.x)
                {
                    rect_.x += dx_;
                }
                else
                {
                    rect_.x -= dx_;
                }
                if (rect_.y > H)
                {
                    dx_ = 2;
                    rect_.x = 32 * col_ + Fleet::rect_.x - 10;
                    rect_.y = 32;
                    state_ = Fleet::fleet_state;
                }
                break;
            case 3: //initialize attack
                attack_initial_x_ = rect_.x;
                destination_x_ = rand() % (W - 32);

                dx_ = 5;

                state_ = 2; //attack state
                break;
        }
    }
}

void PurpleAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}

Image YellowAlien::image_("images/galaxian/GalaxianFlagship.gif");

YellowAlien::YellowAlien(int x, int y, int row, int col)
    : Alien(x, y, row, col)
{ 
    rect_ = image_.getRect();
    rect_.x = x;
    rect_.y = y;
    score_ = 60;
}

void YellowAlien::run()
{
    if (isAlive_)
    {
        switch (state_)
        {
            case 0: //passive in fleet, moving right
                rect_.x += dx_;
                if (rect_.y < row_ * 32 + 32)
                {
                    rect_.y += dy_;
                }
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
                if (rect_.y < row_ * 32 + 32)
                {
                    rect_.y += dy_;
                }

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
                rect_.y += dy_;
                if (rect_.x - 75 < destination_x_
                        && destination_x_ < rect_.x + 75)
                {
                    dx_ = 1;
                }
                if (rect_.x < destination_x_)
                {
                    rect_.x += dx_;
                }
                else
                {
                    rect_.x -= dx_;
                }
                if (rect_.y > H)
                {
                    dx_ = 2;
                    rect_.x = 32 * col_ + Fleet::rect_.x - 10;
                    rect_.y = 32;
                    state_ = Fleet::fleet_state;
                }
                break;
            case 3: //prepare attack
                destination_x_ = getTicks() % (W - 32);

                state_ = 2;
                break;
        }
    }
}

void YellowAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
}


Image PlayerShip::image_("images/galaxian/GalaxianGalaxip.gif");

PlayerShip::PlayerShip(int x, int y)
    : isAlive_(true),
    dx_(5),
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
void PlayerShip::init()
{
    rect_.x = W / 2;
    rect_.y = H - 64;
    isAlive_ = true;
    time_of_death_ = 0;
}

Laser::Laser(int x, int y)
    : dx_(0),
    dy_(-8),
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

void Laser::set(int x, int y, int dy)
{
    rect_.x = x;
    rect_.y = y;
    dy_ = dy;
    isAlive_ = true;
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

Image GameStats::ship_image_("images/galaxian/GalaxianGalaxip.gif");
GameStats::GameStats()
    : num_lives_(3),
    score_(0),
    current_level_(1),
    font_("fonts/FreeMono.ttf", 20),
    score_text_(font_.render("Score: ", WHITE)),
    level_text_(font_.render("Level: ", WHITE)),
    score_number_(font_.render("0", WHITE))
{
    ship_rect_ = ship_image_.getRect();
    ship_rect_.x = 0;
    ship_rect_.y = H - ship_rect_.h;

    
    score_text_rect_ = score_text_.getRect();
    score_text_rect_.x = 0;
    score_text_rect_.y = 0;

    score_number_rect_ = score_number_.getRect();
    score_number_rect_.x = W / 2;
    score_number_rect_.y = 0;

    level_text_rect_ = level_text_.getRect();
    level_text_rect_.x = W - level_text_rect_.w;
    level_text_rect_.y = H - level_text_rect_.h;
    
}
void GameStats::init()
{
    num_lives_ = 3;
    score_ = 0;
    current_level_ = 1;
}
void GameStats::draw(Surface & surface)
{
    //draw lives in lower left
    for (int i = 0; i < num_lives_ - 1; ++i)
    {
        surface.put_image(ship_image_, ship_rect_);
        ship_rect_.x += ship_rect_.w;
    }
    ship_rect_.x = 0;

    
    //draw score in top left
    surface.put_image(score_text_, score_text_rect_);

    int old_score = score_;
    for (int i = MAX_SCORE_DIGITS - 1; i >= 0; --i)
    {
        score_number_char_[i] = old_score % 10 + '0';
        //std::cout << 'a' << old_score % 10 << std::endl;
        old_score /= 10;
    }
    /*
    std::cout << score_number_char_ << std::endl;
    std::cout << 1 << std::endl;
    score_number_ = font_.render(score_number_char_, RED);
    std::cout << 2 << std::endl;
    score_number_rect_ = score_number_.getRect();
    std::cout << 3 << std::endl;
    surface.put_image(score_number_, score_number_rect_);
    std::cout << 4 << std::endl;
    */

    //draw levels in bottom right
    surface.put_image(level_text_, level_text_rect_);
    
}
