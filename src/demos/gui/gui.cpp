#include <sun.hpp>

class GUI : public sun::Application
{
public:

    GUI(sun::Context& context) : sun::Application(context)
    {
        auto gui = context_.get_system<sun::GUISystem>();
    }

    void on_update()
    {
    }

    void on_event(sun::Event& e) override
    {
    }

private:
};

SUN_DEFINE_MAIN_APP(GUI);
