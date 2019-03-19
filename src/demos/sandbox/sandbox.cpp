#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

    struct vertex_def
    {
    };

	sandbox() : sun::application(), font_size(16) {
        renderer_->set_color(sun::color::black);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        texture_ = renderer_->create_texture();

        sun::image img;
        img.load("res/bototem.png");
        fnt_.load("res/mono.ttf");

        texture_->load(img);
        sprt_.set_texture(texture_);
	}

    ~sandbox() {
    }

	void on_event(sun::event& e) override {
        sun::application::on_event(e);

	    if (e.type == sun::event_type::key_pressed) {
            if (e.key.code == sun::keyboard::key::RIGHT) {
                speed_.x += 4.f;
            }
            if (e.key.code == sun::keyboard::key::LEFT) {
                speed_.x -= 4.f;
            }
            if (e.key.code == sun::keyboard::key::UP) {
                speed_.y -= 4.f;
            }
            if (e.key.code == sun::keyboard::key::DOWN) {
                speed_.y += 4.f;
            }
        }
        if (e.type == sun::event_type::key_released) {
            if (e.key.code == sun::keyboard::key::ADD) {
                ++font_size;
                on_font_size_change();
            }
            if (e.key.code == sun::keyboard::key::SUBTRACT) {
                --font_size;
                on_font_size_change();
            }
        }
        if (e.type == sun::event_type::text_entered) {
            fnt_.get_glyph(e.text_input.text[0], font_size);
            sun_printf("%s", e.text_input.text);
        }
	}

	void on_font_size_change() {
	    sun_printf("font size: %d", font_size);
	}

    void on_update() override {
        renderer_->clear();

        if (speed_.x > 0.f) {
            speed_.x -= 0.2f;
        } else if (speed_.x < 0.f) {
            speed_.x += 0.2f;
        }
        if (speed_.y > 0.f) {
            speed_.y -= 0.2f;
        } else if (speed_.y < 0.f) {
            speed_.y += 0.2f;
        }

        //sprt_.set_texture(fnt_.get_page_texture(font_size));

        transform_.translate(speed_);
        renderer_->set_model_transform(transform_);
        renderer_->draw(sprt_);
    }

private:

    sun::sprite sprt_;
    sun::texture* texture_;
    sun::font fnt_;
    sun::matrix4 transform_;
    sun::vector2f speed_;

    int font_size;
};

SUN_DEFINE_MAIN_APP(sandbox)
