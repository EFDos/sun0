#include <sun.hpp>
#include <graphics.hpp>
#include <gui.hpp>

class GUI : public sun::Application
{
public:

    GUI(sun::Context& context) : sun::Application(context)
    {
        renderer_->set_projection(sun::Matrix4::orthogonal(0, 1280, 720, 0));
        gui_ = context_.register_system<sun::GUISystem>();
        auto bar = new sun::Bar(context_, sun::Bar::Orientation::Top);
        //bar->add_child(new sun::TextButton(context));
        //gui_->add_widget(bar);
    }

    void on_update(float delta)
    {
    }

    void on_event(sun::Event& e) override
    {
        sun::Application::on_event(e);
    }

private:
};

SUN_DEFINE_MAIN_APP(GUI);
