#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

	sandbox(sun::context& p_context)
	:   sun::application(p_context),
	    shape2D_(p_context),
	    text_(p_context),
	    font_(p_context),
	    music_(p_context)
	{
        renderer_->set_color(sun::color::dark_grey);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));

        font_.load("res/Wattauchimma.ttf");

        text_.set_font(&font_);
        text_.set_color(sun::color::sun);
        text_.set_character_size(72);
        text_.set_text("Hello\n   Sun!");

        shape2D_.set_color(sun::color::sun);
        shape2D_.set_shape(sun::shapes::circle(400.f, 240));

        music_.load("res/ontar2.ogg");
        music_.set_loop(true);
        music_.set_volume(100.f);
        music_.play();
	}

    ~sandbox() {
    }

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
	}

    void on_update() override {
        if (sun::keyboard::is_key_pressed(sun::keyboard::key::RIGHT)) {
	        speed_.x += 0.7f;
	    }

	    if (sun::keyboard::is_key_pressed(sun::keyboard::key::LEFT)) {
	        speed_.x -= 0.7f;
	    }

        if (sun::keyboard::is_key_pressed(sun::keyboard::key::UP)) {
	        speed_.y -= 0.7f;
	    }

	    if (sun::keyboard::is_key_pressed(sun::keyboard::key::DOWN)) {
	        speed_.y += 0.7f;
	    }

        if (speed_.x > 0.f) {
            speed_.x -= 0.2f;
        } else if (speed_.x < 0.f) {
            speed_.x += 0.2f;
        }
        if (speed_.y > 0.f) {
            speed_.y -= 0.2f;
        } else if (speed_.y < 0.1f) {
            speed_.y += 0.2f;
        }

        transform_.translate(speed_);
        renderer_->set_model_transform(transform_);
        renderer_->clear();
        renderer_->draw(text_);
        renderer_->draw(shape2D_);
    }

private:

    sun::shape2D    shape2D_;
    sun::text       text_;
    sun::font       font_;
    sun::matrix4    transform_;
    sun::vector2f   speed_;

    sun::sound_stream music_;
};

SUN_DEFINE_MAIN_APP(sandbox)
