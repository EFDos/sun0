#include <sun.hpp>

class sandbox : public sun::application
{
public:

	sandbox(sun::context& p_context)
	:   sun::application(p_context),
	    entity_(p_context),
	    background_entity_(p_context),
	    font_(p_context)
	{
        renderer_->set_color(sun::color::dark_grey);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        texture_ = renderer_->create_texture();

        font_.load("res/Wattauchimma.ttf");

        auto sprite_batch = background_entity_.create_component<sun::sprite_batch>();

        auto sprite_component = entity_.create_component<sun::sprite>();

        auto child_entity = entity_.create_child();
        auto shape_component = child_entity->create_component<sun::shape2D>();
        auto text_component = child_entity->create_component<sun::text>();
        child_entity->set_position(64, 64);

        texture_->load(sun::image("res/bototem.png"));

        sprite_batch->set_texture(texture_);

        for (int x = 0 ; x < (1280 / 64) ; ++x) {
            for (int y = 0 ; y < (720 / 64) ; ++y) {
                sprite_batch->add_sprite_rect({(float)x * 64.f, (float)y * 64.f}, {0, 0, 64, 64});
            }
        }

        shape_component->set_shape(sun::shapes::circle(16.f, 32));
        shape_component->set_color(sun::color::sun);
        sprite_component->set_texture(texture_);
        sprite_component->set_rect({0, 0, 64, 64});
        text_component->set_font(&font_);
        text_component->set_text("Hello Sun!");
	}

    ~sandbox() {
    }

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
	}

    void on_update() override {
        entity_.move(0.2f, 0.2f);
        entity_.rotate(0.2f);
    }

private:

    sun::entity     entity_;
    sun::entity     background_entity_;
    sun::texture*   texture_;
    sun::font       font_;
};

SUN_DEFINE_MAIN_APP(sandbox)
