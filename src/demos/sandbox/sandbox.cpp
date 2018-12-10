#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

	void on_event(sun::event& e) {
        sun_print("Type \'close\' to exit");
        char buf[24];
        scanf("%23s", buf);
        if (!strcmp(buf, "close")) {
        	e.type = sun::event_type::closed;
        }

        sun::application::on_event(e);
	}

    void on_update() override {
        sun_log("Hello Sun!");
    }
};

SUN_DEFINE_MAIN_APP(sandbox)
