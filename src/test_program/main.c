#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "../extension_exports.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define wait(n) Sleep(n * 1000)
#else
#include <unistd.h>
#define wait(n) sleep(n)
#endif

void print_result(bool result) {
    printf("%s\n", result ? "passed" : "failed");
    fflush(stdout);
}

bool default_settings(void) {
    printf("default settings\n");

    mp_init();
    if (!mp_setup()) return false;
    if (!mp_play("test.mid")) return false;
    mp_set_gain(1.0);
    mp_wait_until_finished();
    mp_unload();
    mp_deinit();

    return true;
}

bool force_alt(void) {
    printf("force alternate soundfont\n");

    mp_init();

    mp_set_use_system_patches(false);
    mp_set_alt_sf_path("test.sf2");

    if (!mp_setup()) return false;
    if (!mp_play("test.mid")) return false;
    mp_set_gain(0.75);
    mp_wait_until_finished();
    mp_unload();
    mp_deinit();

    return true;
}

bool abrupt_change(void) {
    printf("abrupt song change\n");

    mp_init();

    if (!mp_setup()) return false;
    if (!mp_play("test.mid")) return false;
    mp_set_gain(0.75);
    wait(3);
    if (!mp_play("test2.mid")) return false;
    mp_wait_until_finished();
    mp_unload();
    mp_deinit();

    return true;
}

int main(int argc, char *argv[]) {
    print_result(default_settings());
    print_result(force_alt());
    print_result(abrupt_change());
    return 0;
}