#ifndef BABEL_H
#define BABEL_H

#include "game.h"
#include "mousebuttoneventlistener.h"
#include "mousebuttonevent.h"

class Babel : public Game, public MouseButtonEventListener
{
public:
    Babel();
    ~Babel();
    
    void run();

private:
    Level * load_level(const string& id);
    Level * load_frontend(const string& id);
    Level * load_menu();
    Level * load_settings();
    Level * load_credits();
    bool onMouseButtonEvent(const MouseButtonEvent& event);
};

#endif
