#include "mp.h"

#include <string.h>
#include <stdlib.h>

static mp_state_t state;

void _mp_set_use_win_gm_patches(bool enabled) {
    state.use_win_gm_patches = enabled;
}

void _mp_set_alt_sf_path(char *path) {
    strcpy(state.alt_sf_path, path);
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

bool _mp_set_bpm(int bpm) {
    if (state.fs_player == NULL)
		return false;

    fluid_player_set_bpm(state.fs_player, bpm);
	return true;
}

int _mp_get_bpm(void) {
    if (state.fs_player == NULL)
		return false;

    return fluid_player_get_bpm(state.fs_player);
}

int _mp_get_division(void) {
    if (state.fs_player == NULL)
		return false;

    return fluid_player_get_division(state.fs_player);
}

int _mp_get_tempo(void) {
    if (state.fs_player == NULL)
		return false;

    return fluid_player_get_midi_tempo(state.fs_player);
}

int _mp_get_length_ticks(void) {
    if (state.fs_player == NULL)
		return false;

    return fluid_player_get_total_ticks(state.fs_player);
}

int _mp_get_pos_ticks(void) {
	if (state.fs_player == NULL)
		return false;

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
    state.use_win_gm_patches = true;

    state.alt_sf_path = (char *)malloc(1024);
    memset(state.alt_sf_path, 0, 1024);

    state.fs_settings = new_fluid_settings();
    state.fs_synth = new_fluid_synth(state.fs_settings);
	/* player will get created on play */
    state.fs_player = NULL;
    state.fs_audio_driver = new_fluid_audio_driver(state.fs_settings,
        state.fs_synth);
}

bool _mp_setup(void) {
#ifdef _WIN32
	char win_gm_sf_path[512];

    if (state.use_win_gm_patches) {
		ExpandEnvironmentStrings(WIN_GM_SF_PATH, win_gm_sf_path, 512);

        if (fluid_is_soundfont(win_gm_sf_path)) {
            int result = fluid_synth_sfload(state.fs_synth,
				win_gm_sf_path, 1);
			if (result == FLUID_FAILED) {
				fprintf(stderr, "using alt soundfont, couldn't load gm.dls\n");
			}
			else return true;
		}
		else {
			fprintf(stderr, "using alt soundfont, gm.dls isn't valid ?\n");
		}
	}
	else {
		printf("using alt soundfont, gm.dls loading manually disabled\n");
	}
#else
	printf("using alt soundfont, running on non-windows platform\n");
#endif
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
	}
	else {
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