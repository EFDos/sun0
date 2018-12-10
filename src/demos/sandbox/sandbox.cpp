#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

	void on_event(sun::event& e) override {
        sun::application::on_event(e);
	}

    void on_update() override {
        //sun_log("Hello Sun!");
    }
};

SUN_DEFINE_MAIN_APP(sandbox)
