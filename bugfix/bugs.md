# Some BUGs in AVR toolchain

## OS: Linux

### GCC 5.3.0 cannot find AVRlibc 1.8.1 files correctly

GCC 5.3.0 searches file `crt{MCU Name}.o` and `lib{MCU Name}.a`.
However AVRlibc has recently renamed `crt{MCU Name}.o` to some
strange name, and removed `lib{MCU Name}.a`.

FIX: There is a program analysis AVRLibc Makefiles and generate a
shell script to create symlinks and empty archives to fix this
problem. See `gen_gen_symlink.c`.

### AVRDUDE set Baud rate unproperly

In AVRDUDE some code is broken. Sometimes it would print

`Floating point exception (core dumped)`

and sometimes it does not dump core, but do nothing.

FIX: Since the broken code are dealing with something special on
Linux, we can disable them and let AVRDUDE treat Linux as normal
UNIX. Add

`#undef __linux__`

at the head of `ser_posix.c` and recompile AVRDUDE.
