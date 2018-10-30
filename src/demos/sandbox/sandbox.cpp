#include <sun.hpp>

int main() {
    __sun_log("Hello from Sun!");
    __sun_logf("version: %s, %s",
        sun::loglevel::info,
        sun::version::string,
        sun::version::codename);

    return 0;
}
