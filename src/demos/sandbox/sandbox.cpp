#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:;

	sandbox()
	:   sun::application(),
	    txt_(context_),
	    fnt_(context_),
	    music_(context_),
	    font_size(16)
	{

	    scale_ = {1.f, 1.f};

        renderer_->set_color(sun::color::dark_grey);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));

        fnt_.load("res/Wattauchimma.ttf");

        txt_.set_font(&fnt_);
        txt_.set_color(sun::color::sun);
        txt_.set_character_size(72);
        txt_.set_text("Hello\n   Sun!");

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

	void on_font_size_change() {
	    sun_printf("font size: %d", font_size);
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

        //renderer_->clear();

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

        if (scale_.x < 10.f) {
            scale_.x += 0.01f;
            scale_.y += 0.01f;
        }

        transform_.translate(speed_);
        transform_.set_scale(scale_);
        renderer_->set_model_transform(transform_);
        renderer_->clear();
        renderer_->draw(txt_);
    }

private:

    sun::text   txt_;
    sun::font fnt_;
    sun::matrix4 transform_;
    sun::vector2f speed_;
    sun::vector2f scale_;

    sun::sound_stream music_;

    int font_size;
};

SUN_DEFINE_MAIN_APP(sandbox)
