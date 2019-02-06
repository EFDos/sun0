#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

	sandbox() : sun::application() {
	}

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
	}

    void on_update() override {
    	renderer_->clear(sun::color::sun);
    }

private:

};

SUN_DEFINE_MAIN_APP(sandbox)
