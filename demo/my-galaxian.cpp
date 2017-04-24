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

    AquaAlien aqua;

    aqua.rect_.x = 50;
    aqua.rect_.y = 50;
    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;
        surface.lock();
        surface.fill(BLACK);
        aqua.draw(surface);
        surface.unlock();
        surface.flip();

        delay(200);
        
    }
}

AquaAlien::AquaAlien()
    : image_("images/galaxian/GalaxianAquaAlien.gif"),
    rect_(image_.getRect())
{ }

void AquaAlien::draw(Surface & surface) 
{
    surface.put_image(image_, rect_);
}
