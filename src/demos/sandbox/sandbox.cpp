#include <sun.hpp>

class sandbox : public sun::application
{
public:

    sandbox() {}

    ~sandbox() {}

    void update() override {
        sun_log("Hello Sun!");
    }
};

SUN_DEFINE_MAIN_APP(sandbox)
