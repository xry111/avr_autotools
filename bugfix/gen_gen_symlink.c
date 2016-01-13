/**
  * Create shell script to create symlink and
  * empty archives for GCC so that it could handle
  * AVRlibc.
  *
  * You have to execute this at
  *         /path/to/avrlibc/source/avr/lib/{child}
  * You should feed this the output of
  *         find -maxdepth 1 -type d
  * and redirect stdout to a shell script. Then
  * copy this script to
  *        {prefix}/avr/lib/{child}
  * and execute it. Only exception is {child}=avr2,
  * you should run the script in
  *        {prefix}/avr/lib
  * since the librarys of avr2 are there.
  */

#include <stdio.h>
#include <string.h>

int get_names(const char *file, char *mcuname, char *crtname){
	static char buf[1000];
	FILE *in = fopen(file, "r");

	mcuname[0] = crtname[0] = 0;

	if (!in)
		return 1;

	while (fscanf(in, "%s", buf)==1) {
		if (!strcmp(buf, "AVR_TARGET")) {
			fscanf(in, "%*s");
			fscanf(in, "%s", mcuname);
		} else if (!strcmp(buf, "AVR_TARGET_CRT")) {
			fscanf(in, "%*s");
			fscanf(in, "%s", crtname);
		}
	}

	fclose(in);
	return !(mcuname[0] && crtname[0]);
}

int main() {
	static char buf[1000];
	static char mcu[1000];
	static char crt[1000];
	puts("#!/bin/sh");
	while (scanf("%s", buf)==1) {
		/* Automake file is smaller. */
		strcat(buf, "/Makefile.am");

		if (get_names(buf, mcu, crt)) {
			fprintf(stderr, "Warning: %s does not exist or"
				"does not contain necessary data.\n", buf);
			continue;
		}

		/* Create link to crt, so GCC can find it. */
		printf("ln -sv %s crt%s.o\n", crt, mcu);
		/* Create a null device lib to make gcc happy. */
		printf("ar -v -q lib%s.a\n", mcu);
	}
	return 0;
}
