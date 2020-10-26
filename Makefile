all: xhotcorners

.PHONY: all clean

xhotcorners: xhotcorners.c
	gcc -o $@ $< -lX11 -Wall

clean:
	rm -f xhotcorners

debug:
	valgrind  -v --leak-check=full --track-origins=yes ./xhotcorners
