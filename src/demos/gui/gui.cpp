#include <sun.hpp>

class GUI : public sun::Application
{
public:

    GUI(sun::Context& context) : sun::Application(context)
    {
        renderer_->set_projection(sun::Matrix4::orthogonal(0, 1280, 720, 0));
        gui_ = context_.register_system<sun::GUISystem>();
    }

    void on_update()
    {
    }

    void on_event(sun::Event& e) override
    {
        sun::Application::on_event(e);
    }

private:
};

SUN_DEFINE_MAIN_APP(GUI);
