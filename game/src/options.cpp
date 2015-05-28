#include "file.h"
#include "options.h"
#include <algorithm>
#include <core/font.h>
#include <core/rect.h>
#include <core/resourcesmanager.h>
#include <core/settings.h>

#define W_BUTTON_BACK 140
#define H_BUTTON_BACK 60
#define BUTTON_SETTING 12
#define X_VOLUME 296

Options::Options(const string& next, const string& texture)
    : Level("options", next), m_texture(nullptr), m_logo(nullptr), m_soundvideo(nullptr),
        m_volume(nullptr), m_arrow(nullptr), m_up_volume(nullptr), m_down_volume(nullptr),
        m_up_resolution(nullptr), m_down_resolution(nullptr), m_back(nullptr)
{
    Environment *env = Environment::get_instance();

    m_texture = env->resources_manager->get_texture(texture);
    m_logo = env->resources_manager->get_texture("res/images/menu/babel-logo.png");
    m_soundvideo = env->resources_manager->get_texture("res/images/menu/sound-video.png");
    m_volume = env->resources_manager->get_texture("res/images/menu/volume.png");
    m_arrow = env->resources_manager->get_texture("res/images/menu/arrow.png");

    double scale = env->canvas->scale();
    shared_ptr<Font> font = env->resources_manager->get_font("res/fonts/exo-2/Exo2.0-Regular.otf");
    env->canvas->set_font(font);
    font->set_size(22 * scale);

    m_back = new Button(this, "back", "res/images/menu/button.png",
        (env->canvas->w() - W_BUTTON_BACK * scale)/2, env->canvas->h() - 149 * scale,
        W_BUTTON_BACK * scale, H_BUTTON_BACK * scale);
    m_back->set_text("Back");

    m_up_volume = new Button(this, "up_volume", "",
        X_VOLUME * scale, (env->canvas->h() - 25 * scale)/2, BUTTON_SETTING * scale,
        BUTTON_SETTING * scale);

    m_down_volume = new Button(this, "down_volume", "",
        X_VOLUME * scale, env->canvas->h()/2, BUTTON_SETTING, BUTTON_SETTING);

    m_up_resolution = new Button(this, "up_resolution", "",
        env->canvas->w()/2 + 140 * scale, (env->canvas->h() - 25 * scale)/2,
        BUTTON_SETTING * scale, BUTTON_SETTING * scale);

    m_down_resolution = new Button(this, "down_resolution", "",
        env->canvas->w()/2 + 140 * scale, (env->canvas->h())/2, BUTTON_SETTING * scale,
        BUTTON_SETTING * scale);

    m_back->add_observer(this);
    m_up_volume->add_observer(this);
    m_down_volume->add_observer(this);
    m_up_resolution->add_observer(this);
    m_down_resolution->add_observer(this);

    add_child(m_back);
    add_child(m_up_volume);
    add_child(m_down_volume);
    add_child(m_up_resolution);
    add_child(m_down_resolution);
}

void
Options::update_coordinates()
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();

    m_back->set_position((env->canvas->w() - W_BUTTON_BACK * scale)/2,
        env->canvas->h() - 149 * scale);
    m_back->set_dimensions(W_BUTTON_BACK * scale, H_BUTTON_BACK * scale);

    m_up_volume->set_position(X_VOLUME * scale, (env->canvas->h() - 25 * scale)/2);
    m_up_volume->set_dimensions(BUTTON_SETTING * scale, BUTTON_SETTING * scale);
    
    m_down_volume->set_position(X_VOLUME * scale, env->canvas->h()/2);
    m_down_volume->set_dimensions(BUTTON_SETTING * scale, BUTTON_SETTING * scale);
    
    m_up_resolution->set_position(env->canvas->w()/2 + 140 * scale,
        (env->canvas->h() - 25*scale)/2);
    m_up_resolution->set_dimensions(BUTTON_SETTING * scale, BUTTON_SETTING * scale);
    
    m_down_resolution->set_position(env->canvas->w()/2 + 140 * scale, (env->canvas->h())/2);
    m_down_resolution->set_dimensions(BUTTON_SETTING * scale, BUTTON_SETTING * scale);
}

void
Options::draw_self(double, double)
{
    Environment *env = Environment::get_instance();
    double scale = env->canvas->scale();
    shared_ptr<Font> font = env->canvas->font();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/settings.ini");

    env->canvas->clear();
    env->canvas->draw(m_texture.get());
    env->canvas->draw(m_logo.get(), (env->canvas->w() - m_logo->w() * scale)/2, 25 * scale);
    env->canvas->draw(m_soundvideo.get(), 189 * scale, 321 * scale);
    env->canvas->draw(m_arrow.get(), X_VOLUME * scale, (env->canvas->h() - 20 * scale)/2);
    env->canvas->draw(m_arrow.get(), env->canvas->w()/2 + 140 * scale,
        (env->canvas->h() - 20 * scale)/2);

    int i, volume = settings->read<int>("Game", "volume", 50);

    for (i = 0; i < (10 - volume/10)*17; i+=17)
    {
        env->canvas->draw(m_volume.get(), Rect(0, 15, 15, 15), (313+i) * scale,
            (env->canvas->h() - 15 * scale)/2);
    }
    for (int j = i; j < i + (volume/10)*17; j+=17)
    {
        env->canvas->draw(m_volume.get(), Rect(0, 0, 15, 15), (313+j) * scale,
            (env->canvas->h() - 15 * scale)/2);
    }

    font->set_size(24 * scale);
    set_position((env->canvas->w() - W_BUTTON_BACK)/2 + 23 * scale, 167 * scale);
    env->canvas->draw("OPTIONS", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    font->set_size(18 * scale);
    set_position((bounding_box().y() + 250) * scale, env->canvas->h()/2 - 37 * scale);
    env->canvas->draw("Volume", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    set_position(env->canvas->w()/2 + 20 * scale, env->canvas->h()/2 - 37 * scale);
    env->canvas->draw("Resolution", bounding_box().x(), bounding_box().y(), Color(170, 215, 190));

    int w = env->canvas->w();
    int h = env->canvas->h();
    string text = std::to_string(w) + " x " + std::to_string(h) + " px";
    set_position(env->canvas->w()/2 + 25 * scale, (env->canvas->h() - 25 * scale)/2);
    env->canvas->draw(text, bounding_box().x(), bounding_box().y(), Color(170, 215, 190));
}

bool
Options::on_message(Object *sender, MessageID id, Parameters)
{
    Button *button = dynamic_cast<Button *>(sender);

    if (id != Button::clickedID or not button)
    {
        return false;
    }

    Environment *env = Environment::get_instance();
    shared_ptr<Settings> settings = env->resources_manager->get_settings("res/settings.ini");

    if (button->id() == "back")
    {
        finish();
    }
    else if (button->id() == "up_resolution" or button->id() == "down_resolution")
    {
        int w = env->canvas->w();
        int h;
        int position = std::find(m_resolutions.begin(), m_resolutions.end(), w) -
            m_resolutions.begin();

        if (button->id() == "up_resolution")
        {
            if (position + 1 < (int) m_resolutions.size())
            {
                position++;
            }
        }
        else
        {
            if (position - 1 >= 0)
            {
                position--;
            }
        }

        w = m_resolutions[position];
        h = w * 3 / 4;

        double scale = (double) w / m_resolutions[m_resolutions.size() - 1];
        env->video->set_resolution(w, h, scale);
        update_coordinates();

        settings->write<int>("Game", "w", w);
        settings->write<int>("Game", "h", h);
        settings->write<double>("Game", "scale", scale);
        settings->save("res/settings.ini");
    }
    else if (button->id() == "up_volume" or button->id() == "down_volume")
    {
        int volume = settings->read<int>("Game", "volume", 50);

        if (button->id() == "up_volume")
        {
            if (volume < 100)
            {
                volume += 10;
            }
        }
        else
        {
            if (volume > 0)
            {
                volume -= 10;
            }
        }

        settings->write<int>("Game", "volume", volume);
        settings->save("res/settings.ini");
    }

    return true;
}