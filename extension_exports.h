#ifndef _EXTENSION_EXPORTS_H
#define _EXTENSION_EXPORTS_H

#ifdef _WIN32
#define LIBEXPORT __declspec(dllexport)
#else
#define LIBEXPORT
#endif

LIBEXPORT double mp_set_use_system_patches(double enabled);

LIBEXPORT double mp_set_alt_sf_path(char *path);

LIBEXPORT double mp_play(char *path);
LIBEXPORT double mp_pause(void);
LIBEXPORT double mp_resume(void);
LIBEXPORT double mp_unload(void);

LIBEXPORT double mp_seek(double ticks);
LIBEXPORT double mp_restart(void);
LIBEXPORT double mp_set_bpm(double bpm);
LIBEXPORT double mp_get_bpm(void);
LIBEXPORT double mp_get_division(void);
LIBEXPORT double mp_get_tempo(void);
LIBEXPORT double mp_get_length_ticks(void);
LIBEXPORT double mp_get_pos_ticks(void);

LIBEXPORT double mp_set_gain(double gain);
LIBEXPORT double mp_get_gain(void);

LIBEXPORT double mp_set_loop_count(double loops);

LIBEXPORT double mp_init(void);
LIBEXPORT double mp_setup(void);
LIBEXPORT double mp_deinit(void);

#endif // _EXTENSION_EXPORTS_H
