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
        renderer_->set_color(sun::color::sun);
        triangle_ = renderer_->create_vertex_buffer(sizeof(vertex_def), 3);
        base_shader_ = renderer_->create_shader("res/flat.glsl");

        if (base_shader_ == nullptr) {
            sun_log_fatal("fodeu");
        }

        vertex_def triangle_verts[] = {
            {-0.5f, 0.f, 0.f, 0.f, 0.f, 1.f},
            {0.f, -0.5f, 0.f, 0.f, 0.f, 1.f},
            {0.5f, 0.f, 0.f, 0.f, 0.f, 1.f},
        };

        triangle_->fill_data(0, 3, triangle_verts);

    	renderer_->clear();

    	renderer_->draw(*triangle_, *base_shader_);
	}

    ~sandbox() {
        delete triangle_;
        delete base_shader_;
    }

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
	}

    void on_update() override {
    }

private:

    sun::vertex_buffer* triangle_;
    sun::shader* base_shader_;
};

SUN_DEFINE_MAIN_APP(sandbox)
