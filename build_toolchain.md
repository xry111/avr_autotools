# HOWTO build AVR cross toolchain on UNIX

This file shows how to build and install, from source code, a
development environment for the AVR MCUs using the GNU toolset.

## Step 0. Setting up environment

### Decide the install prefix

At first decide where the cross toolchain would be installed.
Several alternate positions are `/usr`, `/usr/local`, `/opt/avr`
and `$HOME/opt/avr`.

`/usr` is the normal install prefix suggested on Linux. If you
are using Linux and have `root` permission, you could use this
prefix.

`/usr/local` is the normal install prefix suggested on traditional
UNIX. If you are using BSD or other legacy UNIX system, and have
`root` permission, you could use this prefix.

`/opt/avr` is an abnormal install prefix. But it has a great
advantage that if you want to uninstall or reinstall the toolchain,
you can simply `rm -rf` it. This is suggested on Mac OS X. To
install toolchain here you also need `root` permission.

If you have no `root` permission, or you do not want to allow other
users to use your toolchain, or you are afraid the installation
would break your system, you could use `$HOME/opt/avr`.

### Setting up the environment varibles

If you have elected `$HOME/opt/avr`, open `$HOME/.bashrc`.
Otherwise open `/etc/bashrc` (need `root` permission). Then add
`export AVR_PREFIX=/prefix/you/elected` in the file.

Then you should run `echo $PATH` to check if the `bin` subdir of
your prefix is already in `$PATH`. If not you have to set them
manually. Add following lines in `bashrc`:

`
export $PATH=$PATH:$AVR_PREFIX/bin
export $MANPATH=$MANPATH:$AVR_PREFIX/share/man
export $INFOPATH=$INFOPATH:$AVR_PREFIX/share/info
`

Normally, `/usr/bin` and `/usr/local/bin` should be in `$PATH`
by default.

## Step 1. Install GNU Binutils for the AVR target

The `binutils` package provides all the low-level utilities
needed in building and manipulating object files.

Visit [download page](http://ftp.gnu.org/gnu/binutils) to fetch
the newest `binutils` source package. For example:

`wget http://ftp.gnu.org/gnu/binutils/binutils-2.25.1.tar.bz2`

Then unpack it:

`
tar -xf binutils-2.25.1.tar.bz2
cd binutils-2.25.1
`

`binutils` developers recommended to configure and build `binutils`
in a subdirectory not to pollute the source code. So issue
`mkdir obj-avr && cd obj-avr`. Then run

`
../configure --prefix=$AVR_PREFIX --target=avr
make
`

to compile Binutils for AVR targets. Some documents recommended
option `--disable-nls` but it seems unnecessary.

After compilation, you could issue `make check -k` to run testsuites.
Few tests may fail because of unknown reasons.

Then get the necessary permission and issue

`make install`

to install Binutils. At last, check your installation by issuing

`avr-ld -v`

It should print something like `GNU ld (GNU Binutils) 2.25.1`. If not,
double check your environment varibles setting.

## Step 2. Install cross GCC

GCC compile C and C++ source code to AVR assembly code.

Fetch newest GCC source package at [download page]
(http://ftp.gnu.org/gnu/gcc). The steps to build `avr-gcc` are
almost same as for `binutils`:

`
tar -xf gcc-5.3.0.tar.bz2
cd gcc-5.3.0
mkdir obj-avr && cd obj-avr
../configure --prefix=$AVR_PREFIX --target=avr \
--enable-languages=c,c++,lto  --with-dwarf2
make
make install
`

`--with-dwarf2` specify that GCC to use DWARF 2 debugging
information as the default since AVR debuggers only support this.
Some documents recommend `--disable-libssp` and `--disable-nls`
but they seem unnecessary. In `--enable-languages` option, `c` and
`lto` are necessary, you may add `fortran` or remove `c++` as your
wish.

At last check your installation with

`avr-gcc -v`

It should output some configurations of `avr-gcc`.

## Step 3. Install AVR LibC

AVR LibC contains subroutines needed to run your program on AVR MCUs.

Fetch newest AVR LibC at [download page]
(http://download.savannah.gnu.org/releases/avr/libc). Then untar it:

`tar -xf avr-libc-1.8.1.tar.gz`

We need to modify `Makefile.am` to ensure the generation of files
needed by GCC:

`
cd avr-libc-1.8.1
find avr/lib -name "Makefile.am" -exec \
sed -i 's/^AVR_TARGET_CRT.*/AVR_TARGET_CRT = crt$(AVR_TARGET).o lib$(AVR_TARGET.a/g;
s/$@/crt$(AVR_TARGET).o/g;
/^$(AVR_TARGET_CRT)/a\\tar -v -q lib$(AVR_TARGET).a' {} \;
find avr/lib -type d -exec automake {}/Makefile \;
`

This could run for a few time. Then you can configure and install AVR
LibC:

`
./configure --prefix=$AVR_PREFIX \
--build=`./config.guess` --host=avr
make
make install
`

## Step 4. Install AVRDUDE

AVRDUDE is an utility to download/upload/manipulate the ROM and
EEPROM contents of AVR MCUs using the in-system programming technique.

Fetch the newest AVRDUDE at [download page]
(http://download.savannah.gnu.org/releases/avrdude/).

Untar the code:
`
tar -xf avrdude-6.2.tar.gz
cd avrdude-6.2
`

There is a fatal BUG in AVRDUDE 6.2 code operating Linux serial ports.
We can disable these code and make AVRDUDE consider Linux as normal
UNIX:

`sed -i '/^#if !defined(WIN32NATIVE)/a #undef __linux__' ser_posix.c`

Then build and install AVRDUDE:

`
mkdir obj-avr && cd obj-avr
../configure --prefix=$AVR_PREFIX
make
make install
`

Check your installation issuing `avrdude`. Then you should see the
usage of AVRDUDE on screen.
