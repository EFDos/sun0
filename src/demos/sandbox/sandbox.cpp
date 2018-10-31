#include <sun.hpp>

int main() {
    //sun::logger::instance().set_show_timestamp(false);
    __sun_log("Hello from Sun!");
    __sun_logf("version: %s, %s", sun::version::string, sun::version::codename);

    __sun_log_d("debug test");

    __sun_logf_w("warning about number %i", 849);

    __sun_log_e("wooow an error!");

    return 0;
}
