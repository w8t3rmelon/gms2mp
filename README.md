# midimaker: 2!
(formerly gms2mp)

a simple midi playback extension for gamemaker powered by 
[fluidsynth](https://github.com/fluidsynth/fluidsynth)

## disclaimer
this extension is still actively in development so there may be some bugs /
breaking changes along the way

## installing
go to the releases tab on the right side, and download the .yymps file from the
latest release (you can also download the .yyp which includes a demo project)

open your gamemaker project and go to tools > import local package. press
"add all" and then "import". the extension should now be installed and ready to
use

## using
see [here](DOCS.md) for documentation and a small example

## compiling
(just keep in mind that the hellspawn of scripts i've made for this all kinda
stink. i'll probably try improving them someday)

### installing required packages

#### ubuntu
```sudo apt install g++-mingw-w64-x86-64-posix g++ cmake ninja-build libpulse-dev libpipewire-0.3-dev python3```

#### fedora
```sudo apt install mingw64-gcc-c++ mingw64-libgomp cmake ninja g++ pulseaudio-libs-devel pipewire-devel python3```

#### arch linux (insert obligatory cliché here)
```sudo pacman -S cmake gcc ninja mingw-w64-gcc mingw-w64-winpthreads mingw-w64-headers libpulse libpipewire python3```

### actually building
run `build_release_native.sh` and `build_release_mingw.sh`. they will compile
both the linux (or whatever platform you're on) and windows builds and place
them in out/native and out/mingw respectively

### making a "local package"
run `packaging/package.py`. it will pull the built dll/so files previously
created through running the build scripts and also generate some files
gamemaker expects in a local package (alot of stuff there is hardcoded atm)

when done, it should create a file named `ext_midim2.yymps` that you can
import into your gamemaker project (tools > import local package)

## special thanks

- [fluidsynth](https://github.com/fluidsynth/fluidsynth) for being the lifeblood
of this entire project
- samwow and cubitoishere for the name idea
- everyone who cheered me on throughout development
