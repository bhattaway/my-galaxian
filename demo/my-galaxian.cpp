#include "my-galaxian.h"

//tests a simple title screen demo
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
            if (getTicks() - Laser::timeOfLastLaser_ > 750)
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


/****************************************************
 * THIS IS THE MAIN GAME
 *******************************************/
void play_galaxian()
{
    Surface surface(W,H);
    Event event;

    Fleet fleet;

    /*********************************
     * INITIALIZATION
     * ********/
    //init fleet
    fleet.init();
    //init lasers and ship
    const int LASER_SIZE = 100;
    PlayerShip ship(0, 0);
    ship.init();
    Laser laser[LASER_SIZE];

    //init explosion
    const int NUM_EXPLOSIONS = 100;
    Explosion explosion[NUM_EXPLOSIONS];

    //init stars
    const int NUM_STARS = 50;
    Star star[NUM_STARS];

    const int FRAME_RATE = 1000 / 30;

    GameStats gamestats;

    //fonts for title screen and game over
    Font font1("fonts/FreeMono.ttf", 24);
    Font font2("fonts/FreeMono.ttf", 22);

    //"WELCOME TO GALAXIAN" image
    Image title_screen(font1.render("WELCOME TO GALAXIAN", GREEN));
    Rect title_screen_rect = title_screen.getRect();
    title_screen_rect.x = W / 2 - title_screen_rect.w / 2;
    title_screen_rect.y = H / 2 - title_screen_rect.h / 2;

    //"created by brandon hattaway" image
    Image my_name(font2.render("created by brandon hattaway", WHITE));
    Rect my_name_rect = my_name.getRect();
    my_name_rect.x = W / 2 - my_name_rect.w / 2;
    my_name_rect.y = H / 2 - my_name_rect.h / 2 + title_screen_rect.h;

    //"press SPACE to begin" image
    Image press_space(font2.render("press SPACE to begin", GREEN));
    Rect press_space_rect = press_space.getRect();
    press_space_rect.x = W / 2 - press_space_rect.w / 2;
    press_space_rect.y = H / 2 - press_space_rect.h / 2 
        + title_screen_rect.h + my_name_rect.h;

    /*
    //"60" image
    Image sixty(font1.render("60", WHITE));
    Rect sixty_rect = sixty.getRect();
    sixty_rect.x = W + 32;
    sixty_rect.y = H - 128;

    //"50" image
    Image fifty(font1.render("50", WHITE));
    Rect fifty_rect = fifty.getRect();
    fifty_rect.x = W + 32;
    fifty_rect.y = H - 96;
    //"40" image
    Image forty(font1.render("40", WHITE));
    Rect forty_rect = forty.getRect();
    forty_rect.x = W + 32;
    forty_rect.y = H - 64;
    //"30" image
    Image thirty(font1.render("30", WHITE));
    Rect thirty_rect = thirty.getRect();
    thirty_rect.x = W + 32;
    thirty_rect.y = H - 32;
    
    Alien * example_aliens [4];

    example_aliens[0] = new YellowAlien(W, H - 128);
    example_aliens[1] = new RedAlien(W, H - 96);
    example_aliens[2] = new PurpleAlien(W, H - 64);
    example_aliens[3] = new AquaAlien(W, H - 32);
    */

    //"GAME OVER" image
    Image game_over(font1.render("GAME OVER", RED));
    Rect game_over_rect = game_over.getRect();
    game_over_rect.x = W / 2 - game_over_rect.w / 2;
    game_over_rect.y = H / 2 - game_over_rect.h / 2;

    /*************
     * GAME LOOP
     * *********************/
    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;

        int frame_start = getTicks(); //used at the end of the loop
                                      //to determine how long to delay
        //handle input
        KeyPressed keypressed = get_keypressed();

        /**********
         * STATE 0: TITLE SCREEN
         * STATE 1: INITIALIZING THE GAME
         * STATE 2: MAIN GAME
         * *******/
        switch (gamestats.game_state_)
        {
    case 0: //TITLE SCREEN

        //if press space, initialize, then start the game
        if (keypressed[SPACE])
        {
            gamestats.game_state_ = 1; //init state
        }

        //make all stars move
        for (int i = 0; i < NUM_STARS; ++i)
        {
            star[i].run();
        }

        //draw title screen
        surface.lock();
        surface.fill(BLACK);

        surface.put_image(title_screen, title_screen_rect);
        surface.put_image(my_name, my_name_rect);
        surface.put_image(press_space, press_space_rect);
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
        gamestats.game_state_ = 2; //go to main game
        break;

    case 2: //main game 
        
        //handling input and moving the Player accordingly
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

            //if SPACE, fire a laser
            if (keypressed[SPACE] && ship.isAlive())
            {
                //time limit for how quickly you can shoot lasers
                if (getTicks() - Laser::timeOfLastLaser_ > 500)
                {
                    //find a laser that is not alive
                    int i = 0;
                    while (laser[i].isAlive())
                    {
                        ++i;
                    }

                    //create the laser
                    laser[i].isAlive() = true;
                    laser[i].rect().x = ship.rect().x + ship.rect().w / 2;
                    laser[i].rect().y = ship.rect().y - laser[i].rect().h + 4;

                    //update laser time constraint
                    Laser::timeOfLastLaser_ = getTicks();
                }
            }
        }
        else //ship is dead
        {
            if (ship.time_of_death_ == 0) //first time in this section
            {
                ship.time_of_death_ = getTicks();
            }

            //takes 3 seconds to respawn
            if (getTicks() - ship.time_of_death_ > 3000)
            {
                //if you have lives, respawn
                if (gamestats.num_lives_ > 0)
                {
                    ship.init();
                }
                else //if you dont have lives, game over, go back to title
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

        //if no more lives, GAME OVER
        if (gamestats.num_lives_ == 0)
        {
            surface.put_image(game_over, game_over_rect);
        }
        surface.unlock();
        surface.flip();

        }//switch statement end

        //calculate delay between frames to keep a constant framerate
        int frame_end = getTicks();
        int delaytime = FRAME_RATE - frame_end + frame_start;
        if (delaytime > 0) delay(delaytime);
    }

    //free memory used by aliens
    fleet.delete_alien();
}

//check to see if two Rects overlap.
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

//initializes the fleet
void Fleet::init()
{
    //row 0 is YellowAliens
    for (int col = 0; col < NUM_COLS; ++col)
    {
        alien[0][col] = new YellowAlien(32 * col, 1 * 32, 0, col);
    }

    //row 1 is RedAliens
    for (int col = 0; col < NUM_COLS; ++col)
    {
        alien[1][col] = new RedAlien(32 * col, 2 * 32, 1, col);
    }

    //row 2 is PurpleAliens
    for (int col = 0; col < NUM_COLS; ++col)
    {
        alien[2][col] = new PurpleAlien(32 * col, 3 * 32, 2, col);
    }

    //rows 3-6 are AquaAliens
    for (int row = 3; row < NUM_ROWS; ++row)
    {
        for (int col = 0; col < NUM_COLS; ++col)
        {
            alien[row][col] = new AquaAlien(32 * col, 32 * row + 32, row, col);
        }
    }

    //make the fleet look better, take away excess aliens.
    //(resembles original Galaxian game)
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

    //change fleet state to moving right
    switch_state(0);

    //initialize fleet rectangle
    rect_.w = alien[0][0]->rect().w * NUM_COLS;
    rect_.h = alien[0][0]->rect().h * NUM_ROWS;
    rect_.x = 10;
    dx_ = 2;
    recalculate_num_aliens_alive();

    time_of_fleet_death_ = 0;
    time_of_last_attack_ = getTicks();

    ++GameStats::current_level_;
}

//free memory used by aliens
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

//runs each alien in the fleet
void Fleet::run()
{
    //if the entire fleet is not dead
    if (num_aliens_alive != 0)
    {
        //if you hit the left boundary, move right
        if (rect_.x < 10)
        {
            switch_state(0);
            dx_ = -dx_;
        }

        //if you hit the right boundary, move left
        if (rect_.x + rect_.w > W)
        {
            switch_state(1);
            dx_ = -dx_;
        }

        //determine if there should be an attack
        //(one attack every second)
        if (getTicks() - time_of_last_attack_ > 1000)
        {
            //generate a random alien for attacking
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

            //reset attack timer
            time_of_last_attack_ = getTicks();
        }

        //run all the aliens
        for (int row = 0; row < NUM_ROWS; ++row)
        {
            for (int col = 0; col < NUM_COLS; ++col)
            {
                alien[row][col]->run();
            }
        }

        //move the fleet rect
        rect_.x += dx_;
    }
    else //every alien is dead
    {
        if(time_of_fleet_death_ == 0) //first time in this block
        {
            delete_alien();
            time_of_fleet_death_ = getTicks();
        }

        //spawn another wave of aliens after 4 seconds
        if(getTicks() - time_of_fleet_death_ > 4000)
        {
            init();
        }
    }
}

//draw all of the aliens
void Fleet::draw(Surface & surface) const
{
    //if the fleet is alive
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

//check and handle collisions between Lasers and Aliens
void Fleet::do_collision(Laser laser[], int laser_size, 
        Explosion explosion [], int explosion_size, GameStats & gamestats)
{
    if (num_aliens_alive != 0) //if the fleet is alive
    {
        //each laser vs each alien
        for (int i = 0; i < laser_size; ++i)
        {
            if (laser[i].isAlive()) //if this laser is alive
            {
                //check each alien
                for (int row = 0; row < NUM_ROWS; ++row)
                {
                    for (int col = 0; col < NUM_COLS; ++col)
                    {
                        //if this alien is alive, and there is a collision
                        if (alien[row][col]->isAlive() 
                          && isCollision(laser[i].rect(), alien[row][col]->rect()))
                        {
                            //find an explosion that is dead
                            int k = 0;
                            while (explosion[k].isAlive())
                                ++k;

                            //set the explosion at the center of the alien
                            explosion[k].set(alien[row][col]->rect().x 
                                    + alien[row][col]->rect().w / 2,
                                    alien[row][col]->rect().y 
                                    + alien[row][col]->rect().h / 2);

                            //add points to score
                            gamestats.score_ += alien[row][col]->score();

                            //kill alien and laser
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

//check and handle collisions between the Player and each alien
void Fleet::do_collision(PlayerShip & ship, Explosion explosion[],
       int explosion_size, GameStats & gamestats)
{
    if (num_aliens_alive != 0) //if the fleet is alive
    {
        //check collision of aliens vs player ship
        for (int row = 0; row < NUM_ROWS; ++row) //go thru each alien
        {
            for (int col = 0; col < NUM_COLS; ++col)
            {
                //if the alien is alive
                //AND the alien is in attack mode
                //(if the alien is in fleet mode, it cannot collide with
                //the player)
                //AND there is a collision between this alien and the player
                //AND the player is alive
                if (alien[row][col]->isAlive()
                   && alien[row][col]->state() == 2
                   && isCollision(alien[row][col]->rect(), ship.rect())
                   && ship.isAlive())
                {
                    //find a dead explosion
                    int k = 0;
                    while (explosion[k].isAlive())
                        ++k;

                    //set that explosion at the middle of this alien
                    explosion[k].set(alien[row][col]->rect().x 
                            + alien[row][col]->rect().w / 2, 
                                     alien[row][col]->rect().y 
                                     + alien[row][col]->rect().h / 2);

                    //find another dead explosion
                    k = 0;
                    while (explosion[k].isAlive())
                        ++k;

                    //set that explosion at the middle of the player ship
                    explosion[k].set(ship.rect().x + ship.rect().w / 2, 
                                     ship.rect().y + ship.rect().h / 2);
                                     

                    //kill alien
                    alien[row][col]->isAlive() = false;

                    //add points to score
                    gamestats.score_ += alien[row][col]->score();

                    //kill ship
                    ship.isAlive() = false;

                    //take away a life
                    --gamestats.num_lives_;

                    recalculate_num_aliens_alive();
                }
            }
        }
    }
}

//recalculates the number of aliens that are currently alive
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

//switches the state of aliens that are moving back and forth in the fleet.
//if the alien is attacking, do NOT change it's state
void Fleet::switch_state(int new_state)
{
    for (int row = 0; row < NUM_ROWS; ++row)
    {
        for (int col = 0; col < NUM_COLS; ++col)
        {
            //if this alien is not attacking, change its state
            if (alien[row][col]->state() != 2) //2 is attack state
            {
                alien[row][col]->state() = new_state;
            }
        }
    }

    //change fleet state as well
    fleet_state = new_state;
}

//returns a reference of the Fleet rect
Rect & Fleet::rect()
{
    return rect_;
}

//parent class for each of the Aliens.
Alien::Alien(int x, int y, int row, int col)
    : row_(row),
    col_(col),
    state_(0),
    isAlive_(true),
    dx_(2),
    dy_(4)
{
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

//AquaAlien, the most numerous alien
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
    //if this alien is alive
    if (isAlive_)
    {
        switch (state_) 
        {
            case 0: //passive in fleet, moving right

                rect_.x += dx_; //move to the right

                if (rect_.y < row_ * 32 + 32) //if this alien is returning
                                              //from an attack, move it
                                              //downwards until it is in
                                              //it's original spot in 
                                              //the fleet
                {
                    rect_.y += dy_;
                }

                //if this alien goes out of bounds, reset it
                if (rect_.x < 0)
                {
                    rect_.x = 0;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                }

                //if (rand() % 100 == 0) state_ = 1;
                break;
            case 1: //passive in fleet, moving left
                //if alien is returning from an attack, move down
                //until it hits original spot
                if (rect_.y < row_ * 32 + 32)
                {
                    rect_.y += dy_;
                }

                //move to the left
                rect_.x -= dx_;

                //if alien goes out of bounds, reset it
                if (rect_.x < 0)
                {
                    rect_.x = 0;
                }
                else if ((rect_.x + rect_.w) > (W - 1))
                {
                    rect_.x = W - 1 - rect_.w;
                }
                break;

            case 2: //attack

                //move downwards
                rect_.y += dy_;

                //if the alien is within 50px of its destination, slow it down
                //to 1 (horizontal movement)
                if (rect_.x - 50 < destination_x_
                        && destination_x_ < rect_.x + 50)
                {
                    dx_ = 1;
                }
                //if alien is within 100px of destination, slow down to 3
                //(horizontal movement)
                else if (rect_.x - 100 < destination_x_
                        && destination_x_ < rect_.x + 100)
                {
                    dx_ = 3;
                }
                //if alien is within 250px of destination, change to 4
                //(horizontal movement)
                else if (rect_.x - 250 < destination_x_
                        && destination_x_ < rect_.x + 250)
                {
                    dx_ = 4;
                }

                //move towards the destination
                if (destination_x_ > rect_.x)
                {
                    rect_.x += dx_;
                }
                else
                {
                    rect_.x -= dx_;
                }

                //if alien goes out of bounds
                if (rect_.y > H)
                {
                    dx_ = 2; //reset horizontal movement to regular 
                            // fleet speed

                    //set coordinates at top of screen
                    rect_.x = 32 * col_ + Fleet::rect_.x - 10;
                    rect_.y = 32;

                    //get proper state
                    state_ = Fleet::fleet_state;
                }
                break;
            case 3: //prepare attack

                //generate random destination
                destination_x_ = rand() % (W - 32);

                //change to attack state
                state_ = 2;
                break;
        }
    }
}

//draws the alien
void AquaAlien::draw(Surface & surface) const
{
    if (isAlive_) surface.put_image(image_, rect_);
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
            //case 0 and case 1 are same as AquaAlien. see comments there.
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

                //move downwards
                rect_.y += dy_;

                //if within 75px, slow down horizontal movement to 1
                if (rect_.x - 75 < destination_x_
                        && destination_x_ < rect_.x + 75)
                {
                    dx_ = 1;
                }

                //move towards destination
                if (rect_.x < destination_x_)
                {
                    rect_.x += dx_;
                }
                else
                {
                    rect_.x -= dx_;
                }

                //if alien goes out of bounds
                if (rect_.y > H)
                {
                    dx_ = 2; //reset horizontal movement

                    //reset coordinates at top of screen
                    rect_.x = 32 * col_ + Fleet::rect_.x - 10;
                    rect_.y = 32;

                    //sync state with fleet 
                    state_ = Fleet::fleet_state;
                }
                break;
            case 3: //prepare attack

                //generate random destination (done with getTicks
                //in order to sync with YellowAlien when traveling
                //in a group)
                destination_x_ = getTicks() % (W - 32);

                //if destination was in the right third of the screen,
                //set it to all the way Right
                if (destination_x_ > 2 * W / 3)
                {
                    destination_x_ = W;
                }
                //if destination was in the left third of the screen,
                //set it to all the way Left
                else if (destination_x_ < W / 3)
                {
                    destination_x_ = 0;
                }
                //if destination was in the middle third of the screen,
                //set it to the middle
                else
                {
                    destination_x_ = W / 2;
                }

                //set state to attack
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
            //case 0 and 1 are same as AquaAlien, refer there for comments
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
                //this is very similar to AquaAlien. refer there
                //for additional comments
                rect_.y += dy_;

                //if within 10px of destination, generate new destination
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

                //same as AquaAlien
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
            //see AquaAlien for comments about case 0 and 1
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
                //exact same as RedAlien attack
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
                //exact same as Red Alien
                destination_x_ = getTicks() % (W - 32);

                if (destination_x_ > W / 2)
                {
                    destination_x_ = W;
                }
                else
                {
                    destination_x_ = 0;
                }
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
    //rect_.y = y;
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

int Laser::timeOfLastLaser_ = 0;
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

Explosion::Explosion()
    : isAlive_(false),
    x_(0),
    y_(0),
    r_(1),
    dr_(1) //similar to dx_ or dy_ for other classes, except this is for radius
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

        //once explosion expands enough, kill it
        if (r_ > 30) isAlive_ = false;
    }
}
void Explosion::draw(Surface & surface) const
{
    if (isAlive_)
    {
        //large red circle on bottom
        surface.put_circle(x_, y_, r_, RED);

        //medium yellow circle
        surface.put_circle(x_, y_, r_ / 1.2, YELLOW);
        
        //small white circle on top
        surface.put_circle(x_, y_, r_ / 2, WHITE);
    }
}
bool & Explosion::isAlive()
{
    return isAlive_;
}

Star::Star(int x, int y)
    : dx_(0),
    dy_(rand() % 4 + 1), //randomize vertical speed
    color_(rand_color())
{
    rect_.x = rand() % W; //randomize position
    rect_.y = rand() % H;
    rect_.w = 1; //1x1 pixel
    rect_.h = 1;
}
void Star::run()
{
    //move downwards
    rect_.y += dy_;

    //if out of bounds, generate a new star at the top
    if (rect_.y > H) 
    {
        rect_.x = rand() % W;
        rect_.y = 0;
        dy_ = rand() % 4 + 1;
        color_ = rand_color();
    }
}
void Star::draw(Surface & surface) const
{
    surface.put_rect(rect_, color_);
}

Image GameStats::ship_image_("images/galaxian/GalaxianGalaxip.gif");
int GameStats::current_level_(1);

//contains useful stats about the game
GameStats::GameStats()
    : num_lives_(3),
    score_(0),
    font_("fonts/FreeMono.ttf", 20), //used for printing "Score" and "Level"
    score_text_(font_.render("Score: ", WHITE)),
    level_text_(font_.render("Level: ", WHITE)),
    score_number_(font_.render("0", WHITE))
{
    //used to print the Lives in the lower left
    ship_rect_ = ship_image_.getRect();
    ship_rect_.x = 0;
    ship_rect_.y = H - ship_rect_.h;

    
    //used to print the score in the upper left
    score_text_rect_ = score_text_.getRect();
    score_text_rect_.x = 5;
    score_text_rect_.y = 0;

    //used to print the score number in the upper left
    score_number_rect_ = score_number_.getRect();
    score_number_rect_.x = W / 2;
    score_number_rect_.y = 0;

    //used to print the "Level:" int he lower right
    level_text_rect_ = level_text_.getRect();
    level_text_rect_.x = W - level_text_rect_.w - 40;
    level_text_rect_.y = H - level_text_rect_.h - 0;
    
}

//initialize the game stats (used at the start of a new game
void GameStats::init()
{
    num_lives_ = 3;
    score_ = 0;
    current_level_ = 1;
}

//draw various things on the screen
void GameStats::draw(Surface & surface)
{
    //draw lives in lower left
    for (int i = 0; i < num_lives_ - 1; ++i)
    {
        surface.put_image(ship_image_, ship_rect_);
        ship_rect_.x += ship_rect_.w;
    }
    ship_rect_.x = 0;

    
    //draw "Score:" in top left
    surface.put_image(score_text_, score_text_rect_);

    //put score into a character array
    int old_score = score_;
    for (int i = MAX_SCORE_DIGITS - 1; i >= 0; --i)
    {
        score_number_char_[i] = old_score % 10 + '0';
        //std::cout << 'a' << old_score % 10 << std::endl;
        old_score /= 10;
    }
    //put the character array into text
    TextSurface score_num(score_number_char_);
    //draw score number in the upper left
    surface.put_text(score_num, 80, 5);

    //put level number into a char array
    int old_level_num = current_level_;
    for (int i = 2; i >= 0; --i)
    {
        level_number_char_[i] = old_level_num % 10 + '0';
        old_level_num /= 10;
    }
    //draw levels in bottom right
    TextSurface level_num(level_number_char_);
    surface.put_image(level_text_, level_text_rect_);
    surface.put_text(level_num, W - 42, H - 27);
    
}
