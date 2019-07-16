#include <sun.hpp>

class Balls : public sun::Application
{
public:

    Balls(sun::Context& p_context) : sun::Application(p_context), scene_(p_context)
    {
        window_.set_title("Balls!");

        renderer_->set_color(sun::Color::DARK_GREY);
        renderer_->set_projection(sun::Matrix4::orthogonal(0, 1280, 720, 0));
        renderer_->set_viewport({0, 0, 1280, 720});

        physics_->set_debug_draw(true);

        auto ground = scene_.create_entity();
        ground->set_position(640, 700);
        auto ground_body = ground->create_component<sun::RigidBody>();
        ground_body->create(sun::shapes::Rectangle(800, 20), sun::RigidBody::Type::Static);

        auto lwall = scene_.create_entity();
        lwall->set_position(240, 400);
        auto lwall_body = lwall->create_component<sun::RigidBody>();
        lwall_body->create(sun::shapes::Rectangle(20, 600), sun::RigidBody::Type::Static);

        auto rwall = scene_.create_entity();
        rwall->set_position(1040, 400);
        auto rwall_body = rwall->create_component<sun::RigidBody>();
        rwall_body->create(sun::shapes::Rectangle(20, 600), sun::RigidBody::Type::Static);

        for (uint i = 0 ; i < 15 ; ++i) {
            auto ball = scene_.create_entity();
            ball->set_position(640, 50 + (i * 32));
            auto body = ball->create_component<sun::RigidBody>();
            body->create(sun::shapes::Circle(16, 84), sun::RigidBody::Type::Dynamic);
            body->set_restitution(0.5f);
            body->set_friction(0.7f);
        }
    }

    void on_event(sun::Event& e) override {
        sun::Application::on_event(e);

        if (e.type == sun::EventType::MouseButtonPressed) {
            auto ball = scene_.create_entity();
            ball->set_position(e.mouse_button_event.x, e.mouse_button_event.y);
            auto body = ball->create_component<sun::RigidBody>();
            body->create(sun::shapes::Circle(16, 84), sun::RigidBody::Type::Dynamic);
            body->set_restitution(0.5f);
            body->set_friction(0.7f);
            body->set_angular_velocity(32.f);
        }
	}

    void on_update(float delta)
    {
    }

private:

    sun::SceneTree scene_;
};

SUN_DEFINE_MAIN_APP(Balls);
