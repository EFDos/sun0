#include <sun.hpp>
#include <cstdio>

class sandbox : public sun::application
{
public:

	sandbox() : sun::application() {
	    auto src_pair = sun::shader_utils::parse_source_pair(sun::filesys::read_file("res/flat.glsl"));

	    sun::gl_shader_stage frag_shader(src_pair.second, sun::shader_stage::type::fragment);
	    sun::gl_shader_stage vert_shader(src_pair.first, sun::shader_stage::type::vertex);

        frag_shader.compile();
        vert_shader.compile();

	    if (frag_shader.get_status() != sun::shader_stage::status::compile_ok) {
	    	sun_logf_error("%s", frag_shader.get_warnings().c_str());
	    } else {
	        sun_log("Compiled ok.");
	    }
	    if (vert_shader.get_status() != sun::shader_stage::status::compile_ok) {
	    	sun_logf_error("%s", vert_shader.get_warnings().c_str());
	    } else {
	        sun_log("Compiled ok.");
	    }
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
