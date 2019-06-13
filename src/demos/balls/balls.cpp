#include <sun.hpp>

class balls : public sun::application
{
public:

    balls(sun::context& p_context) : sun::application(p_context), scene_(p_context)
    {
        window_.set_title("Balls!");

        renderer_->set_color(sun::color::dark_grey);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        renderer_->set_viewport({0, 0, 1280, 720});

        auto ground = scene_.create_entity();
        ground->set_position(640, 700);
        auto ground_body = ground->create_component<sun::rigid_body>();
        ground_body->create(sun::shapes::rectangle({800, 20}), sun::rigid_body::type::static_body);

        auto lwall = scene_.create_entity();
        lwall->set_position(240, 400);
        auto lwall_body = lwall->create_component<sun::rigid_body>();
        lwall_body->create(sun::shapes::rectangle({20, 600}), sun::rigid_body::type::static_body);

        auto rwall = scene_.create_entity();
        rwall->set_position(1040, 400);
        auto rwall_body = rwall->create_component<sun::rigid_body>();
        rwall_body->create(sun::shapes::rectangle({20, 600}), sun::rigid_body::type::static_body);

        for (uint i = 0 ; i < 4 ; ++i) {
            auto ball = scene_.create_entity();
            ball->set_position(640 + (i * 8), 310);
            auto body = ball->create_component<sun::rigid_body>();
            body->create(sun::shapes::circle(16, 84), sun::rigid_body::type::dynamic_body);
        }
    }

    void on_event(sun::event& e) override {
        sun::application::on_event(e);

        if (e.type == sun::event_type::mouse_button_pressed) {
            auto ball = scene_.create_entity();
            ball->set_position(e.mouse_button.x, e.mouse_button.y);
            auto body = ball->create_component<sun::rigid_body>();
            body->create(sun::shapes::circle(16, 84), sun::rigid_body::type::dynamic_body);
        }
	}

    void on_update()
    {
    }

private:

    sun::scene_tree scene_;
};

SUN_DEFINE_MAIN_APP(balls);
