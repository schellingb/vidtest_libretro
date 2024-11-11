# Video Test for Libretro

Video mode test core for libretro frontends

## Building
vidtest_libretro has no dependency requirements besides a C compiler and the standard library.

### Windows
Open `vidtest_libretro.sln` in Visual Studio (all versions from 2012 and up are supported) and press
build to build the `vidtest_libretro.dll` core file.

### Linux
To build under linux, make sure GCC (package gcc) and GNU Make (package make) are installed.  
Change into the project directory then run `make` to build it.  
You can edit the simple Makefile to set a different compiler or add hardware specific compiler flags.

## License
vidtest_libretro is available under the [GNU General Public License, version 2 or later](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
