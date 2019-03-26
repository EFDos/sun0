#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:;

	sandbox()
	:   sun::application(),
	    shape2D_(context_),
	    txt_(context_),
	    fnt_(context_),
	    music_(context_)
	{
        renderer_->set_color(sun::color::dark_grey);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));

        fnt_.load("res/Wattauchimma.ttf");

        txt_.set_font(&fnt_);
        txt_.set_color(sun::color::sun);
        txt_.set_character_size(72);
        txt_.set_text("Hello\n   Sun!");

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
        renderer_->draw(txt_);
        renderer_->draw(shape2D_);
    }

private:

    sun::shape2D shape2D_;
    sun::text   txt_;
    sun::font fnt_;
    sun::matrix4 transform_;
    sun::vector2f speed_;

    sun::sound_stream music_;
};

SUN_DEFINE_MAIN_APP(sandbox)
