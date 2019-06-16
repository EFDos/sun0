#include <sun.hpp>

class sandbox : public sun::application
{
public:

	sandbox(sun::context& p_context)
	:   sun::application(p_context),
	    scene_(p_context)
	{
        renderer_->set_color(sun::color::dark_grey);
        renderer_->set_viewport({0, 0, 1280, 720});
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        auto texture = renderer_->create_texture();

        auto res_cache = context_.get_system<sun::resource_cache>();
        res_cache->set_path("res");
        //auto font = res_cache->get_resource<sun::font>("Wattauchimma.ttf");
        auto img = res_cache->get_resource<sun::image>("player_128.png");

        texture->load(*img);

        auto ground = scene_.create_entity();
        entity_ = scene_.create_entity();

        ground->set_position(0, 600);
        auto ground_body = ground->create_component<sun::rigid_body>();

        ground_body->create(sun::shapes::rectangle(1280, 32),
            sun::rigid_body::type::static_body);

        auto sprite = entity_->create_component<sun::sprite>();
        auto ent_body = entity_->create_component<sun::rigid_body>("body");
        auto camera = entity_->create_component<sun::camera>();
        camera->set_follow(true);
        camera->set_follow_speed(1.f);

        ent_body->create(sun::shapes::convex({{-32, 18}, {0, -36}, {32, 18}}),
            sun::rigid_body::type::dynamic_body);
        sprite->set_texture(texture);
        entity_->set_origin(48, 56);
        entity_->set_scale(0.75f, 0.75f);
	}

    ~sandbox() {
    }

	void on_event(sun::event& e) override {
        sun::application::on_event(e);

        if (e.type == sun::event_type::key_pressed) {
            if (e.key.code == sun::keyboard::key::RIGHT) {
                entity_->get_component<sun::rigid_body>("body")->apply_linear_impulse({10.f, 0.f});
            }
        }
	}

    void on_update() override {
    }

private:

    sun::scene_tree scene_;
    sun::entity* entity_;
};

SUN_DEFINE_MAIN_APP(sandbox)
