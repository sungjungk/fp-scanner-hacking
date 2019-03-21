CFLAGS_GLIB = $(shell pkg-config --cflags --libs glib-2.0)

CFLAGS = -Wall -Werror 

all: de-obfuscator

de-obfuscator: de-obfuscator.c
	gcc $< -o $@ $(CFLAGS) $(CFLAGS_GLIB)

clean:
	rm -f de-obfuscator *.pgm

.PHONY: all clean
