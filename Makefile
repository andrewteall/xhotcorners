all: xhotcorners

.PHONY: all clean

xhotcorners: xhotcorners.c
	gcc -o $@ $< -lX11 -Wall

clean:
	rm -f xhotcorners
