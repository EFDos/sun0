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
        auto img_ = res_cache->get_resource<sun::image>("bototem.png");


        auto sprite_batch = scene_.get_root().create_component<sun::sprite_batch>();
        auto text = scene_.get_root().create_component<sun::text>();

        texture_->load(*img_);

        sprite_batch->set_texture(texture_);

        for (int x = 0 ; x < (1280 / 64) ; ++x) {
            for (int y = 0 ; y < (720 / 64) ; ++y) {
                sprite_batch->add_sprite_rect({(float)x * 64.f, (float)y * 64.f}, {0, 0, 64, 64});
            }
        }

        text->set_font(font_.get());
        text->set_text("Hello Sun!");
	}

    ~sandbox() {
    }

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
        if (e.type == sun::event_type::key_pressed) {
            scene_.get_root().move(2.f, 0.f);
        }
	}

    void on_update() override {
    }

private:

    sun::scene_tree scene_;
    sun::texture*   texture_;
};

SUN_DEFINE_MAIN_APP(sandbox)
