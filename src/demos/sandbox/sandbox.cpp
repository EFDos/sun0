#include <sun.hpp>

class sandbox : public sun::application
{
public:

	sandbox(sun::context& p_context)
	:   sun::application(p_context),
	    scene_(p_context)
	{
        renderer_->set_color(sun::color::dark_grey);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        texture_ = renderer_->create_texture();

        auto res_cache = context_.get_system<sun::resource_cache>();
        res_cache->set_path("res");
        auto font_ = res_cache->get_resource<sun::font>("Wattauchimma.ttf");
        auto img_ = res_cache->get_resource<sun::image>("player_128.png");


        auto sprite = scene_.get_root().create_component<sun::sprite>();
        auto text = scene_.get_root().create_component<sun::text>();
        auto body = scene_.get_root().create_component<sun::rigid_body>();

        body->create(sun::shapes::rectangle(64, 64), sun::rigid_body::type::dynamic_body);

        texture_->load(*img_);

        sprite->set_texture(texture_);

        text->set_font(font_.get());
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
    sun::texture*   texture_;
};

SUN_DEFINE_MAIN_APP(sandbox)
