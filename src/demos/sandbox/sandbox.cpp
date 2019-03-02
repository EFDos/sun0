#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

    struct vertex_def
    {
        float x, y;
        float r, g, b, a;
    };

	sandbox() : sun::application() {
        renderer_->set_color(sun::color::black);
        renderer_->set_projection(sun::matrix4::orthogonal(0, 1280, 720, 0));
        quad_ = renderer_->create_vertex_buffer(sizeof(vertex_def), 4);
        indices_ = renderer_->create_index_buffer(6);
        texture_ = renderer_->create_texture();

        sun::image img;
        img.load("res/bototem.png");

        texture_->load(img);

        vertex_def quad_verts[] = {
            {0.f , 0.f, 1.f, 1.f, 1.f, 1.f},
            {64.f, 0.f, 1.f, 1.f, 1.f, 1.f},
            {64.f, 64.f, 1.f, 1.f, 1.f, 1.f},
            {0.f , 64.f, 1.f, 1.f, 1.f, 1.f},
        };

        uint indices_data[] = {
            0, 1, 3,
            1, 2, 3
        };

        quad_->fill_data(0, 4, quad_verts);
        indices_->fill_data(0, 6, indices_data);

    	renderer_->clear();

        texture_->bind();
    	renderer_->draw_indexed(*quad_, *indices_, nullptr);
	}

    ~sandbox() {
        delete quad_;
        delete indices_;
    }

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
	}

    void on_update() override {
    }

private:

    sun::vertex_buffer* quad_;
    sun::index_buffer* indices_;
    sun::texture* texture_;
};

SUN_DEFINE_MAIN_APP(sandbox)
