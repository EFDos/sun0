#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

    struct vertex_def
    {
        float x, y;
        float u, v;
        float r, g, b, a;
    };

	sandbox() : sun::application(), font_size(16) {
        renderer_->set_color(sun::color::black);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        quad_ = renderer_->create_vertex_buffer(sizeof(vertex_def), 4);
        indices_ = renderer_->create_index_buffer(6);
        texture_ = renderer_->create_texture();

        sun::image img;
        img.load("res/bototem.png");
        fnt_.load("res/mono.ttf");

        texture_->load(img);

        vertex_def quad_verts[] = {
            {0.f , 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f},
            {512.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f},
            {512.f, 512.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f},
            {0.f , 512.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f},
        };

        uint indices_data[] = {
            0, 1, 3,
            1, 2, 3
        };

        quad_->fill_data(0, 4, quad_verts);
        indices_->fill_data(0, 6, indices_data);

        //texture_->bind();
	}

    ~sandbox() {
        delete quad_;
        delete indices_;
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

        transform_.translate(speed_);
        renderer_->set_model_transform(transform_);
        fnt_.get_page_texture(font_size)->bind();
    	renderer_->draw_indexed(*quad_, *indices_, nullptr);
    }

private:

    sun::vertex_buffer* quad_;
    sun::index_buffer* indices_;
    sun::texture* texture_;
    sun::font fnt_;
    sun::matrix4 transform_;
    sun::vector2f speed_;

    int font_size;
};

SUN_DEFINE_MAIN_APP(sandbox)
