# basic example
this can be placed in either a room's creation code or an object's create event

```gml
mp_set_use_system_patches(true);

// you can optionally set an alternate soundfont for when system patches can't
// be loaded, or if you want to use your own soundfont
//mp_set_alt_sf_path($"{working_directory}my_awesome_soundfont.sf2")

if (!mp_setup())
{
    // failed to load system patches and/or the alternate soundfont..
    exit;
}

// working_directory is prepended to all of these paths because it wouldn't
// work on linux otherwise (i genuinely don't know why)

// let 'er rip!
if (!mp_play($"{working_directory}my_awesome_song.mid") {
    // midi file wasn't found or couldn't be loaded
    exit;
}

// -1 means to loop infinitely
mp_set_loop_count(-1)
```

# what are "system patches"?
by default, this extension attempts to use the midi patches that may or may not
already be installed on your system. in the case of windows, these patches
reside in a file called `gm.dls`, which is what windows' default midi synth
uses and will generally be in the same path on pretty much every windows
installation.

linux is a bit more complicated in this regard, as most distributions do not
come with any of this. some distributions let you install packages that contain
soundfonts, usually installing them to /usr/share/soundfonts. some of these
packages also link "/usr/share/soundfonts/default.sf2" to their soundfont,
which is what this extension will look for by default.

# api reference

## general functions

### mp_init
> [!CAUTION]
> you shouldn't need to call this function manually! the extension will
> automatically call it when the game starts

initialises the internal state and creates the fluidsynth settings, audio
driver and synth objects

### mp_setup
returns: boolean

by default, this will attempt to load system patches, falling back to the
alternate soundfont if one is provided. if either the system patches or the
alternate soundfont loads successfully, `true` is returned. otherwise, if
the loading of both fails, `false` is returned and no midi playback should
be attempted.

if loading system patches is disabled, it will immediately look for the
alternate soundfont and return `true` if it loads successfully. `false` is
returned otherwise.

mp_setup can be called any number of times if you want to try multiple
alternate soundfonts, like so:
```gml
function try_setup()
{
    mp_set_alt_sf_path($"{working_directory}my_awesome_soundfont.sf2")
    if (mp_setup()) return true;

    mp_set_alt_sf_path($"{working_directory}my_less_awesome_soundfont.sf2")
    if (mp_setup()) return true;

    mp_set_alt_sf_path($"{working_directory}my_boring_soundfont.sf2")
    if (mp_setup()) return true;

    return false;
}
```

### mp_deinit
> [!CAUTION]
> you shouldn't need to call this function manually! the extension will
> automatically call it when the game quits

deinitialises the internal state, stops playback and deletes the fluidsynth
settings, synth, and audio driver objects

## settings

### mp_set_use_system_patches
argument(s): boolean

> [!IMPORTANT]
> if you need to call this and/or [mp_set_alt_sf_path](#mp_set_alt_sf_path),
> make sure you do before you call [mp_setup](#mp_setup)

sets whether or not [mp_setup](#mp_setup) should use system patches. see its
documentation for more info

this setting is set to `true` by default

### mp_set_alt_sf_path
argument(s): string

sets the path to an alternate soundfont that [mp_setup](#mp_setup) can look
for. see its documentation for more info

this setting is optional, it doesn't have a value by default

## playback functions

### mp_play
argument(s): string

returns: boolean

tries playing a midi file from the specified path. returns `true` on success,
`false` if the file isn't found or can't be loaded. if called during playback,
stops the current playback first before loading the file and playing it

### mp_pause
returns: boolean

pauses playback. returns `false` if nothing is playing right now

### mp_resume
returns: boolean

resumes playback. returns `false` if nothing is playing right now

### mp_unload
completely stops playback, resets the synth and unloads the current midi file

### mp_seek
argument(s): int

returns: boolean

seeks to a specified "tick" in the currently playing file. if nothing is
playing right now, returns `false`.

### mp_restart
returns: boolean

restarts playback. alias for `mp_seek(0)`

### mp_set_bpm
argument(s): real

returns: boolean

sets the currently playing file's bpm. this will be overrided if a tempo
change event occurs in the midi file. returns `false` if nothing is playing
right now

### mp_get_bpm
returns: real

returns the current bpm value of the currently playing file. returns `0` if
nothing is playing right now

### mp_get_division
returns: int

returns the currently playing file's time division (pulses/quarter). returns
`0` if nothing is playing right now

### mp_get_tempo
returns: int

returns the currently playing file's raw tempo value. returns `0` if nothing is
playing right now

### mp_get_length_ticks
returns: int

returns the length of the currently playing file in ticks. returns `0` if
nothing is playing right now

### mp_get_pos_ticks
returns: int

returns the current position during playback. returns `0` if nothing is playing
right now

### mp_set_loop_count
argument(s): int

returns: boolean

sets the loop count for the currently playing file. `-1` means to loop
infinitely. returns `false` if nothing is playing right now

## miscellaneous functions

### mp_set_gain
argument(s): real

sets the volume of the synthesiser, ranging from 0.0 to 10.0

### mp_get_gain
returns: real

gets the current volume of the synthesiser

### mp_wait_until_finished
blocks until playback of the current file has finished
