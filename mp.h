#ifndef _MP_H
#define _MP_H

#include <stdbool.h>
#include <fluidsynth.h>

#define WIN_GM_SF_PATH "%SystemRoot%\\System32\\drivers\\gm.dls"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

typedef struct {
	bool					use_win_gm_patches;
	char					*alt_sf_path;

	fluid_settings_t		*fs_settings;
	fluid_synth_t			*fs_synth;
	fluid_player_t			*fs_player;
	fluid_audio_driver_t	*fs_audio_driver;
} mp_state_t;

extern mp_state_t state;

/* set whether or not to use the general midi sound bank included in windows 
   (gm.dls) for playing midis. default value is true. if the value is false
   or gm.dls can't be found, the alternate soundfont will be used instead. */
void _mp_set_use_win_gm_patches(bool enabled);

/* set the path to an alternate soundfont file. by default, the alternate 
   soundfont is only used if gm.dls can't be found. */
void _mp_set_alt_sf_path(char *path);

bool _mp_play(char *path);
bool _mp_pause(void);
bool _mp_resume(void);
bool _mp_unload(void);

bool _mp_seek(int ticks);
bool _mp_restart(void);
bool _mp_set_bpm(int bpm);
int _mp_get_bpm(void);
int _mp_get_division(void);
int _mp_get_tempo(void);
int _mp_get_length_ticks(void);
int _mp_get_pos_ticks(void);

void _mp_set_gain(float gain);
float _mp_get_gain(void);

bool _mp_set_loop_count(int loops);

void _mp_init(void);
bool _mp_setup(void);
void _mp_deinit(void);

#endif /* _MP_H */