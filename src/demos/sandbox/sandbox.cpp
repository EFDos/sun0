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
        auto font = res_cache->get_resource<sun::font>("Wattauchimma.ttf");
        auto img = res_cache->get_resource<sun::image>("player_128.png");

        texture->load(*img);

        auto ground = scene_.create_entity();
        auto entity = scene_.create_entity();

        ground->set_position(0, 600);
        auto ground_body = ground->create_component<sun::rigid_body>();

        ground_body->create(sun::shapes::rectangle(1280, 32),
            sun::rigid_body::type::static_body);

        auto sprite = entity->create_component<sun::sprite>();
        auto text = entity->create_component<sun::text>();
        auto ent_body = entity->create_component<sun::rigid_body>();
        auto camera = entity->create_component<sun::camera>();
        camera->set_follow(true);

        ent_body->create(sun::shapes::rectangle(64, 64),
            sun::rigid_body::type::dynamic_body);
        sprite->set_texture(texture);
        text->set_font(font.get());
        text->set_text("Hello Sun!");
        text->set_character_size(72);
	}

    ~sandbox() {
    }

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
	}

    void on_update() override {
    }

private:

    sun::scene_tree scene_;
};

SUN_DEFINE_MAIN_APP(sandbox)
