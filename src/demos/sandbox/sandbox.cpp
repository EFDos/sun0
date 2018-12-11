#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

	void on_event(sun::event& e) override {
        sun::application::on_event(e);

        if (e.type == sun::event_type::text_entered) {
        	sun_printf("Mouse moved to %s", e.text_input.text);
        }
	}

    void on_update() override {
        //sun_log("Hello Sun!");
    }
};

SUN_DEFINE_MAIN_APP(sandbox)
