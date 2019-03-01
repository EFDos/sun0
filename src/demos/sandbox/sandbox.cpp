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
        renderer_->set_color(sun::color::white);
        quad_ = renderer_->create_vertex_buffer(sizeof(vertex_def), 4);
        indices_ = renderer_->create_index_buffer(6);

        vertex_def quad_verts[] = {
            {-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f},
            { 0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f},
            { 0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f},
            {-0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f},
        };

        uint indices_data[] = {
            0, 1, 3,
            1, 2, 3
        };

        quad_->fill_data(0, 3, quad_verts);
        indices_->fill_data(0, 6, indices_data);

    	renderer_->clear();

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
};

SUN_DEFINE_MAIN_APP(sandbox)
