#include "mp.h"

#include <string.h>
#include <stdlib.h>

static mp_state_t state;

#ifdef _WIN32
/* this is my shitcode :D ~mira */
static bool is_using_wine() {
    HMODULE kernel32 = LoadLibrary("kernel32.dll");
    if (kernel32 == NULL) {
        fprintf(stderr, "couldn't detect wine, failed to load kernel32.dll");
        return false;
    }

    LPSTR (*CDECL wine_get_unix_file_name_ptr)(LPCWSTR)
        = GetProcAddress(kernel32, "wine_get_unix_file_name");

    bool using_wine = wine_get_unix_file_name_ptr != NULL;

    FreeLibrary(kernel32);

    printf("wine detected? %s\n", using_wine ? "yes" : "no");
    return using_wine;
}
#endif

void _mp_set_use_system_patches(bool enabled) {
    state.use_system_patches = enabled;
}

void _mp_set_alt_sf_path(char *path) {
    size_t length = strlen(path);

    if (state.alt_sf_path != NULL) {
        free(state.alt_sf_path);
    }

    state.alt_sf_path = malloc(length + 1);
    memcpy(state.alt_sf_path, path, length + 1);
}

bool _mp_play(char *path) {
    if (state.fs_player != NULL)
        _mp_unload();

	if (fluid_is_midifile(path)) {
        state.fs_player = new_fluid_player(state.fs_synth);
        int result = fluid_player_add(state.fs_player, path);
		if (result == FLUID_FAILED) {
			fprintf(stderr, "can't play, fluid_player_add failed :(\n");
            _mp_unload();
			return false;
		}
		else {
            int result = fluid_player_play(state.fs_player);
			if (result == FLUID_FAILED) {
				fprintf(stderr, "can't play, fluid_player_play failed :(\n");
                _mp_unload();
				return false;
			}
			return true;
		}
	}
	else {
		fprintf(stderr, "can't play, midi file isn't valid :(\n");
		return false;
	}
}

bool _mp_pause(void) {
    if (state.fs_player == NULL)
		return false;

    fluid_player_stop(state.fs_player);
	return true;
}

bool _mp_resume(void) {
    if (state.fs_player == NULL)
		return false;

    fluid_player_play(state.fs_player);
	return true;
}

bool _mp_unload(void) {
    fluid_synth_all_notes_off(state.fs_synth, -1);
    if (state.fs_player != NULL) {
        delete_fluid_player(state.fs_player);
        state.fs_player = NULL;
	}
	return true;
}

bool _mp_seek(int ticks) {
    if (state.fs_player == NULL)
		return false;

    return fluid_player_seek(state.fs_player, 0) != FLUID_FAILED;
}

bool _mp_restart(void) {
    return _mp_seek(0);
}

bool _mp_set_bpm(double bpm) {
    if (state.fs_player == NULL)
		return false;

    fluid_player_set_tempo(state.fs_player,
                           FLUID_PLAYER_TEMPO_EXTERNAL_BPM, bpm);
	return true;
}

double _mp_get_bpm(void) {
    if (state.fs_player == NULL)
		return false;

    return 60000000.0 / (double)_mp_get_tempo();
}

int _mp_get_division(void) {
    if (state.fs_player == NULL)
        return 0;

    return fluid_player_get_division(state.fs_player);
}

int _mp_get_tempo(void) {
    if (state.fs_player == NULL)
        return 0;

    return fluid_player_get_midi_tempo(state.fs_player);
}

int _mp_get_length_ticks(void) {
    if (state.fs_player == NULL)
        return 0;

    return fluid_player_get_total_ticks(state.fs_player);
}

int _mp_get_pos_ticks(void) {
	if (state.fs_player == NULL)
        return 0;

	return fluid_player_get_current_tick(state.fs_player);
}

void _mp_set_gain(float gain) {
    fluid_synth_set_gain(state.fs_synth, gain);
}

float _mp_get_gain(void) {
    return fluid_synth_get_gain(state.fs_synth);
}

bool _mp_set_loop_count(int loops) {
    if (state.fs_player == NULL)
        return false;

    fluid_player_set_loop(state.fs_player, loops);
    return true;
}

void _mp_init(void) {
    state.use_system_patches = true;

    state.alt_sf_path = NULL;

    state.fs_settings = new_fluid_settings();
    state.fs_synth = new_fluid_synth(state.fs_settings);
	/* player will get created on play */
    state.fs_player = NULL;
    state.fs_audio_driver = new_fluid_audio_driver(state.fs_settings,
        state.fs_synth);
}

static bool mp_load_system_patches(void) {
    if (!state.use_system_patches) {
        printf("can't use system patches, loading them was explicitly "
               "disabled\n");
        return false;
    }

#ifdef _WIN32
    /* using Z:\ might also not be the best idea but eh ~mira */
    if (is_using_wine()) {
        printf("wine detected, attempting to get default.sf2 from Z:\\\n");

        if (!fluid_is_soundfont(WINE_DEFAULT_SF_PATH)) {
            fprintf(stderr, "can't use system patches, default.sf2 isn't valid "
                            "according to fluidsynth ?\n");
            return false;
        }

        int result = fluid_synth_sfload(state.fs_synth,
                                        WINE_DEFAULT_SF_PATH, 1);
        if (result == FLUID_FAILED) {
            fprintf(stderr, "can't use system patches, couldn't load "
                            "default.sf2\n");
            return false;
        }

        return true;
    } else {
        char sys_sf_path[512];

        printf("attempting to use gm.dls for system patches\n");

        ExpandEnvironmentStrings(WIN_GM_SF_PATH, sys_sf_path, 512);

        if (!fluid_is_soundfont(sys_sf_path)) {
            fprintf(stderr, "can't use system patches, gm.dls isn't valid "
                            "according to fluidsynth ?\n");
            return false;
        }

        int result = fluid_synth_sfload(state.fs_synth,
                                        sys_sf_path, 1);
        if (result == FLUID_FAILED) {
            fprintf(stderr, "can't use system patches, couldn't load gm.dls\n");
            return false;
        }

        return true;
    }
#else
    /* i've found that a lot of soundfont packages (atleast on fedora) install
       to /usr/share/soundfonts and sometimes add a default.sf2 symlink..
       atleast using that's better than nothing but if you have a better method
       of going about this please let me know ~mira */
    printf("attempting to use default.sf2 for system patches\n");

    if (!fluid_is_soundfont(LIN_DEFAULT_SF_PATH)) {
        fprintf(stderr, "can't use system patches, default.sf2 isn't valid "
                        "according to fluidsynth ?\n");
        return false;
    }

    int result = fluid_synth_sfload(state.fs_synth,
                                    LIN_DEFAULT_SF_PATH, 1);
    if (result == FLUID_FAILED) {
        fprintf(stderr, "can't use system patches, couldn't load "
                        "default.sf2\n");
        return false;
    }

    return true;
#endif
}

bool _mp_setup(void) {
    if (mp_load_system_patches()) {
        printf("using system patches ! :D\n");
        return true;
    }

    printf("system patches not loaded, trying alt soundfont\n");

    if (state.alt_sf_path == NULL) {
		fprintf(stderr, "no alt soundfont path supplied, bye.. :(\n");
		return false;
	}

    if (fluid_is_soundfont(state.alt_sf_path)) {
        int result = fluid_synth_sfload(state.fs_synth,
            state.alt_sf_path, 1);
		if (result == FLUID_FAILED) {
			fprintf(stderr, "couldn't load alt soundfont, bye.. :(\n");
			return false;
		}
		else return true;
    } else {
		fprintf(stderr, "alt soundfont isn't valid, bye.. :(\n");
		return false;
	}
}

void _mp_deinit(void) {
    free(state.alt_sf_path);

    delete_fluid_audio_driver(state.fs_audio_driver);
    if (state.fs_player != NULL)
        delete_fluid_player(state.fs_player);
    delete_fluid_synth(state.fs_synth);
    delete_fluid_settings(state.fs_settings);
}
