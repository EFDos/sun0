#include <sun.hpp>
#include <graphics.hpp>
#include <resources.hpp>

class Hello : public sun::Application
{
private:

    sun::Matrix4    transform_;

public:

	Hello(sun::Context& p_context)
	:   sun::Application(p_context)
	{
        renderer_->set_color(sun::Color::DARK_GREY);
        renderer_->set_viewport({0, 0, 1280, 720});
        renderer_->set_projection(sun::Matrix4::orthogonal(0, 1280, 720, 0));

        auto res_cache = context_.get_system<sun::ResourceCache>();
        res_cache->set_path("res");
        auto font = res_cache->get_resource<sun::Font>("Wattauchimma.ttf");

        auto text = context_.create_component<sun::Text>();

        text->set_font(font.get());
        text->set_character_size(64);
        text->set_text("Hello Sun!");
        text->set_transform(&transform_);

        transform_.translate(1280 / 2 - text->get_bounding_rect().w / 2,
                             720 / 2 - text->get_bounding_rect().h / 2);
	}

    ~Hello() {
    }

	void on_event(sun::Event& e) override {
        sun::Application::on_event(e);
	}

    void on_update(float delta) override {
        //sun_logf("Delta time: %f", delta);
    }
};

SUN_DEFINE_MAIN_APP(Hello)
