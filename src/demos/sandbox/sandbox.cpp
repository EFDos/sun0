#include <sun.hpp>

class sandbox : public sun::application
{
public:

	sandbox(sun::context& p_context)
	:   sun::application(p_context),
	    entity_(p_context)
	{
        renderer_->set_color(sun::color::dark_grey);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        texture_ = renderer_->create_texture();

        auto sprite_component = entity_.create_component<sun::sprite>();

        sprite_component = renderer_->create_component<sun::sprite>();

        texture_->load(sun::image("res/bototem.png"));

        sprite_component->set_texture(texture_);
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
};

SUN_DEFINE_MAIN_APP(sandbox)
