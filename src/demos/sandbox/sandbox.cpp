#include <sun.hpp>

class sandbox : public sun::application
{
public:

	sandbox(sun::context& p_context)
	:   sun::application(p_context),
	    entity_(p_context),
	    font_(p_context)
	{
        renderer_->set_color(sun::color::dark_grey);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        texture_ = renderer_->create_texture();

        font_.load("res/Wattauchimma.ttf");

        auto sprite_component = entity_.create_component<sun::sprite>();
        auto text_component = entity_.create_component<sun::text>();
        auto shape_component = entity_.create_component<sun::shape2D>();

        texture_->load(sun::image("res/bototem.png"));

        sprite_component->set_texture(texture_);
        text_component->set_font(&font_);
        text_component->set_text("Hello Sun!");
        shape_component->set_shape(sun::shapes::circle(16.f, 32));
	}

    ~sandbox() {
    }

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
	}

    void on_update() override {
        entity_.move(0.2f, 0.2f);
    }

private:

    sun::entity     entity_;
    sun::texture*   texture_;
    sun::font       font_;
};

SUN_DEFINE_MAIN_APP(sandbox)
