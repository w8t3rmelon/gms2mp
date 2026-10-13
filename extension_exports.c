#include "mp.h"
#include "extension_exports.h"

LIBEXPORT double mp_set_use_win_gm_patches(double enabled) {
    _mp_set_use_win_gm_patches(enabled == 1.0 ? true : false);
    return 1.0;
}

LIBEXPORT double mp_set_alt_sf_path(char *path) {
    _mp_set_alt_sf_path(path);
    return 1.0;
}

LIBEXPORT double mp_play(char *path) {
    return _mp_play(path) ? 1.0 : 0.0;
}
LIBEXPORT double mp_pause(void) {
    return _mp_pause() ? 1.0 : 0.0;
}
LIBEXPORT double mp_resume(void) {
    return _mp_resume() ? 1.0 : 0.0;
}
LIBEXPORT double mp_unload(void) {
    return _mp_unload() ? 1.0 : 0.0;
}

LIBEXPORT double mp_seek(double ticks) {
    return _mp_seek((int)ticks) ? 1.0 : 0.0;
}
LIBEXPORT double mp_restart(void) {
    return _mp_restart() ? 1.0 : 0.0;
}
LIBEXPORT double mp_set_bpm(double bpm) {
    return _mp_set_bpm((int)bpm) ? 1.0 : 0.0;
}
LIBEXPORT double mp_get_bpm(void) {
    return (double)_mp_get_bpm();
}
LIBEXPORT double mp_get_division(void) {
    return (double)_mp_get_division();
}
LIBEXPORT double mp_get_tempo(void) {
    return (double)_mp_get_tempo();
}
LIBEXPORT double mp_get_length_ticks(void) {
    return (double)_mp_get_length_ticks();
}
LIBEXPORT double mp_get_pos_ticks(void) {
    return (double)_mp_get_pos_ticks();
}

LIBEXPORT double mp_set_gain(double gain) {
    _mp_set_gain((float)gain);
    return 1.0;
}
LIBEXPORT double mp_get_gain(void) {
    return (double)_mp_get_gain();
}

LIBEXPORT double mp_set_loop_count(double loops) {
    return _mp_set_loop_count((int)loops) ? 1.0 : 0.0;
}

LIBEXPORT double mp_init(void) {
    _mp_init();
    return 1.0;
}
LIBEXPORT double mp_setup(void) {
    return _mp_setup() ? 1.0 : 0.0;
}
LIBEXPORT double mp_deinit(void) {
    _mp_deinit();
    return 1.0;
}