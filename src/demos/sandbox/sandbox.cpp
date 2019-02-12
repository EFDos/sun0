#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

	sandbox() : sun::application() {
        renderer_->set_color(sun::color::sun);
	}

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
	}

    void on_update() override {
    	renderer_->clear();
    }

private:

};

SUN_DEFINE_MAIN_APP(sandbox)
