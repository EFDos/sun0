#include <sun.hpp>

class Sandbox : public sun::Application
{
public:

	Sandbox(sun::Context& p_context)
	:   sun::Application(p_context),
	    scene_(p_context)
	{
        renderer_->set_color(sun::Color::DARK_GREY);
        renderer_->set_viewport({0, 0, 1280, 720});
        renderer_->set_projection(sun::Matrix4::orthogonal(0, 1280, 720, 0));
        auto texture = renderer_->create_texture();

        auto res_cache = context_.get_system<sun::ResourceCache>();
        res_cache->set_path("res");
        //auto font = res_cache->get_resource<sun::font>("Wattauchimma.ttf");
        auto img = res_cache->get_resource<sun::Image>("player_128.png");

        texture->load(*img);

        auto ground = scene_.create_entity();
        entity_ = scene_.create_entity();

        ground->set_position(0, 600);
        auto ground_body = ground->create_component<sun::RigidBody>();

        ground_body->create(sun::shapes::Rectangle(1280, 32),
            sun::RigidBody::Type::Static);

        auto sprite = entity_->create_component<sun::Sprite>();
        auto ent_body = entity_->create_component<sun::RigidBody>("body");
        auto camera = entity_->create_component<sun::Camera>();
        auto light = entity_->create_component<sun::Light2D>();
        camera->set_follow(true);
        camera->set_follow_speed(1.f);

        light->set_intensity(600.f);

        ent_body->create(sun::shapes::Convex({{-32, 18}, {0, -36}, {32, 18}}),
            sun::RigidBody::Type::Dynamic);
        sprite->set_texture(texture);
        entity_->set_origin(48, 56);
        entity_->set_scale(0.75f, 0.75f);
	}

    ~Sandbox() {
    }

	void on_event(sun::Event& e) override {
        sun::Application::on_event(e);

        if (e.type == sun::EventType::KeyPressed) {
            if (e.key_event.code == sun::keyboard::Key::Right) {
                entity_->get_component<sun::RigidBody>("body")->apply_linear_impulse({10.f, 0.f});
            }
        }
	}

    void on_update(float delta) override {
        sun_logf("Delta time: %f", delta);
    }

private:

    sun::SceneTree scene_;
    sun::Entity* entity_;
};

SUN_DEFINE_MAIN_APP(Sandbox)
