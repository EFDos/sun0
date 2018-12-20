#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

	sandbox() : sun::application() {
		window_.create("Sun On Haiku", 1280, 720);
	}

	void on_event(sun::event& e) override {
        sun::application::on_event(e);

        if (e.type == sun::event_type::text_entered) {
        	sun_printf("Mouse moved to %s", e.text_input.text);
        }
	}

    void on_update() override {
    	glClearColor(0.f, 0.f, 0.f, 1.f);
    	glClear(GL_COLOR_BUFFER_BIT);

    	glBegin(GL_TRIANGLES);
    	glColor3f(1.f, 1.f, 1.f);
    	glVertex3f(0, 0.5, 0);
    	glVertex3f(0.5, -0.5, 0);
    	glVertex3f(-0.5, -0.5, 0);
    	glEnd();
    }

private:

	GLuint vbo;
	GLuint vao;
};

SUN_DEFINE_MAIN_APP(sandbox)
