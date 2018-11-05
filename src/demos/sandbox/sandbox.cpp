#include <sun.hpp>

class Sandbox : public sun::application
{
public:

    Sandbox() {}

    ~Sandbox() {}

    int run() {
        __sun_log("Hello Sun!");
        return 0;
    }
};

SUN_DEFINE_MAIN_APP(Sandbox)
